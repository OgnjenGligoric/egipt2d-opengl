/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include <vector>
#include <cstdlib> 
#include <ctime>

using namespace std;

SpriteRenderer* Renderer;
GameObject* Player;
GameObject* Sun;
GameObject* Moon;
GameObject* Desert;
GameObject* Sky;
GameObject* Star;
vector<GameObject*> Stars;
GameObject* Water;
GameObject* Fish;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
    
}

Game::Game()
{
}

Game::~Game()
{
    delete Renderer;
    delete Player;
    delete Sun;
    delete Moon;
    delete Desert;
    delete Sky;
    delete Water;
    delete Fish;

    for (const auto& star : Stars) {
        delete star;
    }
}

void Game::Init()
{
    // load shaders
    ResourceManager::LoadShader("sprite.vert", "sprite.frag", nullptr, "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // load textures
    ResourceManager::LoadTexture("res/texel_checker.png", false, "face");
    ResourceManager::LoadTexture("res/sun.png", true, "sun");
    ResourceManager::LoadTexture("res/moon.png", true, "moon");
    ResourceManager::LoadTexture("res/desert.png", true, "desert");
    ResourceManager::LoadTexture("res/sky.png", true, "sky");
    ResourceManager::LoadTexture("res/star.png", true, "star");
    ResourceManager::LoadTexture("res/water_shaped.png", true, "water");
    ResourceManager::LoadTexture("res/fish.png", true, "fish");

    Sun = new GameObject(glm::vec2(this->Width-200.0f, this->Height / 2.0f - 100.0f), glm::vec2(200.0f, 200.0f), ResourceManager::GetTexture("sun"));
    Moon = new GameObject(glm::vec2(0.0f, this->Height / 2.0f - 100.0f), glm::vec2(200.0f, 200.0f), ResourceManager::GetTexture("moon"));
    Desert = new GameObject(glm::vec2(0.0f, Height/2.0f), glm::vec2(Width, Height/2.0f), ResourceManager::GetTexture("desert"));
    Sky = new GameObject(glm::vec2(0.0f, 0.0f), glm::vec2(Width, Height), ResourceManager::GetTexture("sky"));
    Water = new GameObject(glm::vec2(Width / 1.5f, Height / 1.2f), glm::vec2(Width/3, Width/10), ResourceManager::GetTexture("water"), glm::vec3(1.0f), glm::vec2(0.0f, 0.0f), 0.7f);
    _initializeStars();
    Fish = new GameObject(glm::vec2(Width / 1.45f, Height / 1.1f), glm::vec2(Width / 30, Width/30), ResourceManager::GetTexture("fish"));

}

void Game::Update(float dt)
{
    _updateSunAndMoon(dt);
    _updateSkyBrightness(dt);
    _moveFish(dt);
}

void Game::ProcessInput(float dt)
{
    if (Keys[GLFW_KEY_R]) {
        _sunAngle = 180.0f;
        _timeSpeed = 50.0f;
    }
    if (Keys[GLFW_KEY_P])
    {
        _timeSpeed = 0.0f;
    }
    if (Keys[GLFW_KEY_S])
    {
        _initializeStars();
    }
    if (Keys[GLFW_KEY_F])
    {
        Fish->FlipHorizontally();
    }
}

void Game::Render()
{
    Sky->Draw(*Renderer);

    for (auto& star : Stars) {
        star->Draw(*Renderer);
    }

    Sun->Draw(*Renderer);
    Moon->Draw(*Renderer);
    Desert->Draw(*Renderer);
    Fish->Draw(*Renderer);
    Water->Draw(*Renderer);
}

void Game::_updateSunAndMoon(float dt)
{
    glm::vec2 circleCenter = glm::vec2(this->Width / 2.0f, _getSunRiseHeightPoint()); 

    _sunAngle += _timeSpeed * dt;       

    if (_sunAngle > 360.0f)
        _sunAngle -= 360.0f;

    float sunRadians = glm::radians(_sunAngle);
    float moonRadians = sunRadians + glm::pi<float>(); 

    Sun->Position.x = circleCenter.x + _getSunRotationRadius() * cos(sunRadians);
    Sun->Position.y = circleCenter.y + _getSunRotationRadius() * sin(sunRadians);

    Moon->Position.x = circleCenter.x + _getSunRotationRadius() * cos(moonRadians);
    Moon->Position.y = circleCenter.y + _getSunRotationRadius() * sin(moonRadians);
}

void Game::_updateSkyBrightness(float dt)
{
    float normalizedHeight = (_getSunRiseHeightPoint() - Sun->Position.y) / _getSunRotationRadius();
    normalizedHeight = glm::clamp(normalizedHeight, 0.0f, 1.0f);

    const glm::vec3 darkestColor = glm::vec3(0.0f, 0.0f, 0.2f); // Midnight blue
    const glm::vec3 brightestColor = glm::vec3(0.5f, 0.7f, 1.0f); // Sky blue

    const glm::vec3 currentColor = glm::mix(darkestColor, brightestColor, normalizedHeight);

    Sky->Color = currentColor;
    const float starVisibility = 1.0f - normalizedHeight; // Stars are visible as the sky darkens
    for (const auto& star : Stars) {
        star->Alpha = starVisibility;
    }
}

void Game::_initializeStars()
{
    srand(static_cast<unsigned>(std::time(nullptr)));
    constexpr int starCount = 50;

    while (Stars.size() < starCount) {
        Stars.push_back(new GameObject(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), ResourceManager::GetTexture("star")));
    }

    for (int i = 0; i < starCount; ++i) {
        const auto x = static_cast<float>(rand() % Width);
        const auto y = static_cast<float>(rand() % static_cast<int>(_getSunRiseHeightPoint()));
        const auto size = static_cast<float>(10 + std::rand() % 21);
        Stars[i]->Position = glm::vec2(x, y);
        Stars[i]->Size = glm::vec2(size, size);
        Stars[i]->Alpha = 1.0f; 
    }
}

void Game::_moveFish(float dt)
{
    const float fishSpeed = 100.0f ; 
    const float padding = Water->Size.x / 10.0f;

    float waterLeft = Water->Position.x + padding;
    float waterRight = Water->Position.x + Water->Size.x - padding;

    if (Fish->Position.x + Fish->Size.x > waterRight) {
        Fish->FlipHorizontally();
        Fish->Position.x = waterRight - Fish->Size.x; 
    }
    else if (Fish->Position.x <= waterLeft) {
        Fish->FlipHorizontally();
        Fish->Position.x = waterLeft; 
    }

    if (!Fish->IsFlippedHorizontally) {
        Fish->Position.x -= fishSpeed * dt; 
    }
    else {
        Fish->Position.x += fishSpeed * dt;
    }
}

float Game::_getSunRiseHeightPoint() const
{
    return this->Height / 1.5f;
}

float Game::_getSunRotationRadius() const
{
    return this->Width / 3.0f;
}
