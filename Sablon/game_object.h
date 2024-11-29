#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"

class GameObject
{
public:
    glm::vec2   Position, Size, Velocity;
    glm::vec3   Color;
    float       Rotation;
    bool        IsSolid;
    bool        Destroyed;
    float       Alpha{};
    float       Threshold{};
    glm::vec3   HighlightColor{};
    bool        IsFlippedHorizontally = false;
    Texture2D   Sprite;
    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f), float alpha = 1.0f, float threshold = 0.0f, glm::vec3 highlightColor = glm::vec3(1.0f, 0.0f, 0.0f));
    virtual void Draw(SpriteRenderer& renderer);
    void FlipHorizontally();
};

#endif