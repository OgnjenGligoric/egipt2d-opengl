#include "game_object.h"


GameObject::GameObject()
    : Position(0.0f, 0.0f),
	Size(1.0f, 1.0f),
	Velocity(0.0f),
	Color(1.0f), Rotation(0.0f),
	IsSolid(false),
	Destroyed(false),
	Threshold(0.0f),
	HighlightColor(glm::vec3(1.0f, 0.0f, 0.0f)) { }

GameObject::GameObject(glm::vec2 pos, 
					glm::vec2 size, 
					Texture2D sprite, 
					glm::vec3 color, 
					glm::vec2 velocity, 
					float alpha, 
					float threshold, 
					glm::vec3 highlightColor)
    : Position(pos),
	Size(size),
	Velocity(velocity),
	Color(color),
	Rotation(0.0f),
	IsSolid(false),
	Destroyed(false),
	Alpha(alpha),
	Threshold(threshold),
	HighlightColor(highlightColor),
	Sprite(sprite) { }

void GameObject::Draw(SpriteRenderer& renderer)
{
    renderer.DrawSprite(this->Sprite, 
		this->Position, 
		this->Size,
		this->Rotation, 
		this->Color, this->Alpha, 
		this->IsFlippedHorizontally, 
		this->Threshold, 
		this->HighlightColor);
}

void GameObject::FlipHorizontally()
{
    IsFlippedHorizontally = !IsFlippedHorizontally;
}
