#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include <SDL.h>
#include "Singleton.h"


class InputManager final : public Singleton<InputManager>
{
public:
    void Update(float elapsedSec);
    bool IsGameRestart();

    Point2f GetMousePosition();
    std::unordered_map<SDL_Keycode, bool>& GetKeysDownMap() { return m_KeysDownMap; };
    std::unordered_map<Uint8, bool>& GetMouseDownMap() { return m_MouseDownMap; };

    void ManageKeyDown(const SDL_KeyboardEvent& event);
    void ManageKeyUp(const SDL_KeyboardEvent& event);
    void ManageMouseDown(const SDL_MouseButtonEvent& event);
    void ManageMouseUp(const SDL_MouseButtonEvent& event);
    void ManageMouseMotion(const SDL_MouseMotionEvent& event);


private:
    friend class Singleton<InputManager>;
    InputManager();
    ~InputManager() = default;

    std::unordered_map<SDL_Keycode, bool> m_KeysDownMap{};
    std::unordered_map<Uint8, bool> m_MouseDownMap{};


};


