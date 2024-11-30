#ifndef GAME_H
#define GAME_H

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "game_object.h"

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Game
{
public:
    GameState               State;
    bool                    Keys[1024];
    unsigned int            Width, Height;
    Game(unsigned int width, unsigned int height);
    Game();
    ~Game();
    void Init();
    void ProcessInput(int key);
    void ProcessMouseClick(double x, double y);
    void Update(float dt);
    bool Render();
private:
    bool _shouldClose = false;
    bool _startOpeningDoors;
    bool _isDisplayedToBeContinued = false;
    float _toBeContinuedThreshold = 0.0f;
    void _openDoors(float dt);
    void _initializeDoors() const;
    void _toggleDoorVisibility();
    float _sunAngle = 180.0f;
    float _timeSpeed = 50.0f;
    void _updateSunAndMoon(float dt);
    void _updateSkyBrightness(float dt) const;
    float _getSunRiseHeightPoint() const;
    float _getSunRotationRadius() const;
    GameObject* GetLargestPyramid() const;
    void _initializeStars() const;
    void _initializePyramids();
    void _initializeGrass() const;
    void _moveFish(float dt);
    void _toggleGrassVisibility();
};

#endif