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
    Sun = new GameObject(glm::vec2(this->Width-200.0f, this->Height / 2.0f - 100.0f), glm::vec2(200.0f, 200.0f), ResourceManager::GetTexture("sun"));
    Moon = new GameObject(glm::vec2(0.0f, this->Height / 2.0f - 100.0f), glm::vec2(200.0f, 200.0f), ResourceManager::GetTexture("moon"));

}

void Game::Update(float dt)
{

}

void Game::ProcessInput(float dt)
{

}

void Game::Render()
{
    Renderer->DrawSprite(ResourceManager::GetTexture("sun"), glm::vec2(200.0f, 200.0f), glm::vec2(Width/10, Width/10), 0.0f);
    Sun->Draw(*Renderer);
    Moon->Draw(*Renderer);
}