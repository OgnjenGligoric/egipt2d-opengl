#pragma once
#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"


class SpriteRenderer
{
public:
    SpriteRenderer(Shader& shader);
    ~SpriteRenderer();
    void DrawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f), float alpha = 1.0f, bool isFlippedHorizontally = false, float threshold = 0.0f, glm::vec3 highlightColor = glm::vec3(1.0f, 0.0f, 0.0f));
private:
    Shader       shader;
    unsigned int quadVAO;
    void initRenderData();
};

#endif