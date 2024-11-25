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

    Sun = new GameObject(glm::vec2(this->Width-200.0f, this->Height / 2.0f - 100.0f), glm::vec2(200.0f, 200.0f), ResourceManager::GetTexture("sun"));
    Moon = new GameObject(glm::vec2(0.0f, this->Height / 2.0f - 100.0f), glm::vec2(200.0f, 200.0f), ResourceManager::GetTexture("moon"));
    Desert = new GameObject(glm::vec2(0.0f, Height/2.0f), glm::vec2(Width, Height/2.0f), ResourceManager::GetTexture("desert"));
    Sky = new GameObject(glm::vec2(0.0f, 0.0f), glm::vec2(Width, Height), ResourceManager::GetTexture("sky"));

    srand(static_cast<unsigned>(std::time(nullptr)));
    constexpr int starCount = 50; 
    for (int i = 0; i < starCount; ++i) {
	    const auto x = static_cast<float>(rand() % Width);
	    const auto y = static_cast<float>(rand() % static_cast<int>(_getSunRiseHeightPoint())); // Only in the upper part of the sky
	    const auto size = static_cast<float>(10 + std::rand() % 21);
    	Stars.push_back(new GameObject(glm::vec2(x, y), glm::vec2(size, size), ResourceManager::GetTexture("star")));
    }
}

void Game::Update(float dt)
{
    _updateSunAndMoon(dt);
    _updateSkyBrightness(dt);
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

float Game::_getSunRiseHeightPoint() {
    return this->Height / 1.5f;
}

float Game::_getSunRotationRadius() {
    return this->Width / 3.0f;
}