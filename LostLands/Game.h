#pragma once
#include "BaseGame.h"

#include <vector>


class InputManager;
class EntityManager;
class LevelManager;
class Texture;
class Entity;
class Player;


class Game : public BaseGame
{
public:
    explicit Game(const Window& window);
    ~Game() = default;

    Game(const Game& other) = delete;
    Game& operator=(const Game& other) = delete;
    Game(Game&& other) = delete;
    Game& operator=(Game&& other) = delete;

    // http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
    void Update(float elapsedSec) override;
    void Draw() const override;

    // Event handling
    void ProcessKeyDownEvent(const SDL_KeyboardEvent& e) override;
    void ProcessKeyUpEvent(const SDL_KeyboardEvent& e)	override;
    void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e) override;
    void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e) override;
    void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e) override;


private:
    // FUNCTIONS
    void ClearBackground() const;


    const std::string m_ResourcePath{};
    std::unique_ptr<Texture> m_uFpsTexture{};

    float m_AccumulatedSec{};
    float m_FPS{};


};


