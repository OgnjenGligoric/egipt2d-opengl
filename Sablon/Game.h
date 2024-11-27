/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAME_H
#define GAME_H

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "game_object.h"

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    // game state
    GameState               State;
    bool                    Keys[1024];
    unsigned int            Width, Height;
    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    Game();
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput(int key);
    void Update(float dt);
    void Render();
private:
    float _sunAngle = 180.0f;
    float _timeSpeed = 50.0f;
    void _updateSunAndMoon(float dt);
    void _updateSkyBrightness(float dt) const;
    float _getSunRiseHeightPoint() const;
    float _getSunRotationRadius() const;
    GameObject* GetLargestPyramid() const;
    void _initializeStars() const;
    void _initializePyramids() const;
    void _initializeGrass() const;
    void _moveFish(float dt);
    void _toggleGrassVisibility();
};

#endif