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

SpriteRenderer* Renderer;
GameObject* Player;
GameObject* Sun;
GameObject* Moon;
GameObject* Desert;
GameObject* Sky;

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

    Sun = new GameObject(glm::vec2(this->Width-200.0f, this->Height / 2.0f - 100.0f), glm::vec2(200.0f, 200.0f), ResourceManager::GetTexture("sun"));
    Moon = new GameObject(glm::vec2(0.0f, this->Height / 2.0f - 100.0f), glm::vec2(200.0f, 200.0f), ResourceManager::GetTexture("moon"));
    Desert = new GameObject(glm::vec2(0.0f, Height/2.0f), glm::vec2(Width, Height/2.0f), ResourceManager::GetTexture("desert"));
    Sky = new GameObject(glm::vec2(0.0f, 0.0f), glm::vec2(Width, Height), ResourceManager::GetTexture("sky"));

}

void Game::Update(float dt)
{
    UpdateSunAndMoon(dt);
    UpdateSkyBrightness(dt);
}

void Game::ProcessInput(float dt)
{
    
}

void Game::Render()
{
    Sky->Draw(*Renderer);
    Sun->Draw(*Renderer);
    Moon->Draw(*Renderer);
    Desert->Draw(*Renderer);
}

void Game::UpdateSunAndMoon(float dt)
{
    glm::vec2 circleCenter = glm::vec2(this->Width / 2.0f, this->Height / 1.5f); 
    float circleRadius = Width / 3.0f;

    static float sunAngle = 180.0f; 
    sunAngle += 50.0f * dt;       

    if (sunAngle > 360.0f)
        sunAngle -= 360.0f;

    float sunRadians = glm::radians(sunAngle);
    float moonRadians = sunRadians + glm::pi<float>(); 

    Sun->Position.x = circleCenter.x + circleRadius * cos(sunRadians);
    Sun->Position.y = circleCenter.y + circleRadius * sin(sunRadians);

    Moon->Position.x = circleCenter.x + circleRadius * cos(moonRadians);
    Moon->Position.y = circleCenter.y + circleRadius * sin(moonRadians);
}

void Game::UpdateSkyBrightness(float dt)
{
    float normalizedHeight = (this->Height / 1.5f - Sun->Position.y) / (this->Height / 3.0f);
    normalizedHeight = glm::clamp(normalizedHeight, 0.0f, 1.0f);

    glm::vec3 darkestColor = glm::vec3(0.0f, 0.0f, 0.2f); // Midnight blue
    glm::vec3 brightestColor = glm::vec3(0.5f, 0.7f, 1.0f); // Sky blue

    glm::vec3 currentColor = glm::mix(darkestColor, brightestColor, normalizedHeight);

    Sky->Color = currentColor; 
}