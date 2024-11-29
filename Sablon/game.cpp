#include "game.h"
#include <algorithm>
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include <vector>
#include <cstdlib> 
#include <ctime>
#include "text_renderer.h"

using namespace std;

SpriteRenderer* Renderer;
GameObject* Player;
GameObject* Sun;
GameObject* Moon;
GameObject* Desert;
GameObject* Sky;
GameObject* Star;
vector<GameObject*> Stars;
vector<GameObject*> Grass;
vector<GameObject*> Pyramids;
vector<GameObject*> Doors;
GameObject* Water;
GameObject* Fish;
TextRenderer* Text;

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
    for (const auto& grass : Grass) {
        delete grass;
    }
    for (const auto& pyramid: Pyramids) {
        delete pyramid;
    }
    for (const auto& door: Doors) {
        delete door;
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
	ResourceManager::LoadTexture("res/grass.png", true, "grass");
	ResourceManager::LoadTexture("res/pyramid.png", true, "pyramid");
	ResourceManager::LoadTexture("res/door.jpg", true, "door");

	Sun = new GameObject(glm::vec2(this->Width - 200.0f, this->Height / 2.0f - 100.0f), glm::vec2(200.0f, 200.0f),
	                     ResourceManager::GetTexture("sun"));
	Moon = new GameObject(glm::vec2(0.0f, this->Height / 2.0f - 100.0f), glm::vec2(200.0f, 200.0f),
	                      ResourceManager::GetTexture("moon"));
	Desert = new GameObject(glm::vec2(0.0f, Height / 2), glm::vec2(Width, Height / 2),
	                        ResourceManager::GetTexture("desert"));
	Sky = new GameObject(glm::vec2(0.0f, 0.0f), glm::vec2(Width, Height), ResourceManager::GetTexture("sky"));
	Water = new GameObject(glm::vec2(Width / 1.5f, Height / 1.2f), glm::vec2(Width / 3, Width / 10),
	                       ResourceManager::GetTexture("water"), glm::vec3(1.0f), glm::vec2(0.0f, 0.0f), 0.7f);
	_initializeStars();
	_initializePyramids();
	_initializeGrass();
	Fish = new GameObject(glm::vec2(Width / 1.45f, Height / 1.1f), glm::vec2(Width / 30, Width / 30),
	                      ResourceManager::GetTexture("fish"));
    Text = new TextRenderer(Width, Height);
    Text->Load("fonts/Antonio-Regular.ttf", 24);
}

void Game::Update(float dt)
{
    _updateSunAndMoon(dt);
    _updateSkyBrightness(dt);
    _moveFish(dt);
    if(_startOpeningDoors)
    {
        _openDoors(dt);
    }
}

void Game::ProcessInput(int key)
{
    if (key == GLFW_KEY_R) {
        _sunAngle = 180.0f;
        _timeSpeed = 50.0f;
    }
    if (key == GLFW_KEY_P)
    {
        _timeSpeed = 0.0f;
    }
    if (key == GLFW_KEY_S)
    {
        _initializeStars();
    }
    if (key == GLFW_KEY_F)
    {
        Fish->FlipHorizontally();
    }
    if (key == GLFW_KEY_3)
    {
        _initializePyramids();
    }
    if (key == GLFW_KEY_O)
    {
        _toggleDoorVisibility();
    }
	if (key == GLFW_KEY_G)
    {
        _initializeGrass();
    }
    if (key == GLFW_KEY_1 || key == GLFW_KEY_2)
    {
	    _toggleGrassVisibility();
    }

    if (Keys[GLFW_KEY_D])
    {
	    GameObject* largestPyramid = GetLargestPyramid();
        largestPyramid->Threshold += 0.01f;
        if (largestPyramid->Threshold > 1.0f) {
            largestPyramid->Threshold = 1.0f;
        }
    }
    if (Keys[GLFW_KEY_A])
    {
        GameObject* largestPyramid = GetLargestPyramid();
        largestPyramid->Threshold -= 0.01f;
        if (largestPyramid->Threshold < 0.0f) {
            largestPyramid->Threshold = 0.0f;
        }
    }
}

void Game::ProcessMouseClick(double x, double y)
{
    for (const auto& door : Doors)
    {
	    if(door->Alpha == 1.0f && door->Position.x <= x && door->Position.x + door->Size.x >= x && door->Position.y <= y && door->Position.y + door->Size.y >= y)
	    {
            _isDisplayedToBeContinued = true;
        }
    }
}

void Game::Render()
{
    Sky->Draw(*Renderer);

    for (const auto& star : Stars)
    {
        star->Draw(*Renderer);
    }

    Sun->Draw(*Renderer);
    Moon->Draw(*Renderer);
    Desert->Draw(*Renderer);
    for (size_t i = 0; i < Pyramids.size(); ++i)
    {
        Pyramids[i]->Draw(*Renderer);
        Doors[i]->Draw(*Renderer);
    }
    Fish->Draw(*Renderer);
    Water->Draw(*Renderer);
    for (const auto& grass: Grass)
    {
        grass->Draw(*Renderer);
    }
    Text->RenderText("Ognjen Gligoric SV79/2021", Width/30, Height/30, 1.0f);

    if (_isDisplayedToBeContinued)
    {
        Text->RenderText("Nastavice se u 3D projektu", Width / 2, Height / 2, 2.0f);
    }
}


void Game::_updateSunAndMoon(float dt)
{
	const auto circleCenter = glm::vec2(this->Width / 2.0f, _getSunRiseHeightPoint()); 

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

void Game::_updateSkyBrightness(float dt) const
{
    float normalizedHeight = (_getSunRiseHeightPoint() - Sun->Position.y) / _getSunRotationRadius();
    normalizedHeight = glm::clamp(normalizedHeight, 0.0f, 1.0f);

    const glm::vec3 darkestColor = glm::vec3(0.0f, 0.0f, 0.3f); // Midnight blue
    const glm::vec3 brightestColor = glm::vec3(0.5f, 0.7f, 1.0f); // Sky blue

    const glm::vec3 currentColor = glm::mix(darkestColor, brightestColor, normalizedHeight);

    Sky->Color = currentColor;
    const float starVisibility = 1.0f - normalizedHeight; 
    for (const auto& star : Stars) {
        star->Alpha = starVisibility;
    }
}

void Game::_initializeStars() const
{
    srand(static_cast<unsigned>(std::time(nullptr)));
    constexpr int starCount = 50;

    while (Stars.size() < starCount) {
        Stars.push_back(new GameObject(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), ResourceManager::GetTexture("star")));
    }

    for (int i = 0; i < starCount; ++i) {
        const auto x = static_cast<float>(rand() % Width);
        const auto y = static_cast<float>(rand() % static_cast<int>(_getSunRiseHeightPoint()));
        const auto size = static_cast<float>(10 + rand() % 21);
        Stars[i]->Position = glm::vec2(x, y);
        Stars[i]->Size = glm::vec2(size, size);
        Stars[i]->Alpha = 1.0f; 
    }
}

void Game::_initializePyramids()
{
    srand(static_cast<unsigned>(std::time(nullptr)));
    constexpr int pyramidCount = 3;

    while (Pyramids.size() < pyramidCount) {
        Pyramids.push_back(new GameObject(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), ResourceManager::GetTexture("pyramid")));
    }

    for (int i = 0; i < pyramidCount; ++i) {
        const auto size = static_cast<float>(Width) / 10 + rand() % 100;
        const auto x = static_cast<float>(rand() % Width/2);
        const auto y = _getSunRiseHeightPoint() - size + rand() % static_cast<int>(Height - _getSunRiseHeightPoint() - size);
        Pyramids[i]->Position = glm::vec2(x, y);
        Pyramids[i]->Size = glm::vec2(size, size);
        Pyramids[i]->Alpha = 1.0f;
        Pyramids[i]->Threshold = 0.0f;
    }

    std::sort(Pyramids.begin(), Pyramids.end(), [](const GameObject* a, const GameObject* b)
    {
        return a->Position.y + a->Size.y < b->Position.y + b->Size.y;
    });

    _initializeDoors();
}

void Game::_initializeGrass() const
{
    srand(static_cast<unsigned>(std::time(nullptr)));
    constexpr int grassCount = 30;

    while (Grass.size() < grassCount) {
        Grass.push_back(new GameObject(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), ResourceManager::GetTexture("grass")));
    }

    for (int i = 0; i < grassCount; ++i) {
        const auto size = static_cast<float>(Width) / 15 + rand() % 200;
        const auto x = Water->Position.x - size/2 + rand() % static_cast<int>(Water->Size.x);
        const auto y = Water->Position.y + Water->Size.y - size +  (rand() % static_cast<int>(Width/30) - Width/60);
        Grass[i]->Position = glm::vec2(x, y);
        Grass[i]->Size = glm::vec2(size, size);
        Grass[i]->Alpha = 1.0f;
    }

    std::sort(Grass.begin(), Grass.end(), [](const GameObject* a, const GameObject* b)
        {
            return a->Position.y + a->Size.y < b->Position.y + b->Size.y;
        });
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

void Game::_toggleGrassVisibility()
{
    for (const auto& grass : Grass)
    {
        grass->Alpha = static_cast<float>((static_cast<int>(grass->Alpha)+1) % 2);
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

auto Game::GetLargestPyramid() const -> GameObject*
{
    return *max_element(Pyramids.begin(), Pyramids.end(), [](const GameObject* a, const GameObject* b) {
        return (a->Size.x * a->Size.y) < (b->Size.x * b->Size.y); 
        });
}

void Game::_initializeDoors() const
{
    Doors.clear(); 
    Doors.shrink_to_fit();
    for (const auto& pyramid : Pyramids)
    {
        Doors.push_back(new GameObject(glm::vec2(pyramid->Position.x+pyramid->Size.x/4, pyramid->Position.y+pyramid->Size.y-pyramid->Size.x/4), glm::vec2(pyramid->Size.x/4,pyramid->Size.x/4), ResourceManager::GetTexture("door")));
    }
    for (const auto& door : Doors)
    {
        door->Alpha = 0.0f;
        door->Rotation = 270.0f;
        door->HighlightColor = glm::vec3(0.0f, 0.0f, 0.0f);
    }
}

void Game::_toggleDoorVisibility()
{
    for (const auto& door : Doors)
    {
        door->Alpha = static_cast<int>(door->Alpha + 1.0f) % 2;
        if (door->Alpha == 1.0f)
        {
            _startOpeningDoors = true;
            door->Threshold = 0.0f;
        }
    }
}

auto Game::_openDoors(float dt) -> void
{
    for (const auto& door : Doors)
    {
        door->Threshold += 0.01f;
        if (door->Threshold > 1.0f) {
            door->Threshold = 1.0f; 
        }
    }
}
