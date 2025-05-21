#include "pch.h"
#include "InputManager.h"

#include "utils.h"
#include "EntityManager.h"
#include "LevelManager.h"


InputManager::InputManager()
{
    std::cout << "Created InputManager" << "\n";


    m_KeysDownMap =
    {
        { SDLK_ESCAPE, false },
        { SDLK_SPACE, false },
        { SDLK_w, false },
        { SDLK_a, false },
        { SDLK_s, false },
        { SDLK_d, false },
        { SDLK_k, false },
        { SDLK_r, false },
        { SDLK_p, false },
        { SDLK_0, false },
        { SDLK_1, false },
        { SDLK_2, false },
        { SDLK_3, false },
        { SDLK_4, false },
        { SDLK_5, false },
        { SDLK_6, false },
        { SDLK_7, false },
        { SDLK_8, false },
        { SDLK_9, false },
    };

    m_MouseDownMap =
    {
        { SDL_BUTTON_LEFT, false },
        { SDL_BUTTON_MIDDLE, false },
        { SDL_BUTTON_RIGHT, false }
    };


}

InputManager::~InputManager()
{
    std::cout << "Destroyed InputManager" << "\n";
}


void InputManager::Update(float elapsedSec)
{
    if (IsGameRestart())
    {
        LevelManager::GetInstance().ResetLevel();
    }
}

bool InputManager::IsGameRestart()
{
    if (auto it{ m_KeysDownMap.find(SDLK_r) }; it != m_KeysDownMap.end())
        if (it->second)
        {
            it->second = false;
            return true;
        }

    return false;
}


void InputManager::ManageKeyDown(const SDL_KeyboardEvent& event)
{
    //std::cout << "KEYd event: " << event.keysym.sym << std::endl;
    if (!event.repeat)
        if (m_KeysDownMap.count(event.keysym.sym))
            m_KeysDownMap[event.keysym.sym] = true;

}



void InputManager::ManageKeyUp(const SDL_KeyboardEvent& event)
{
    //std::cout << "KEYUP event: " << event.keysym.sym << std::endl;
    if (m_KeysDownMap.count(event.keysym.sym))
        m_KeysDownMap[event.keysym.sym] = false;

    if (event.keysym.sym == SDLK_p)
        LevelManager::GetInstance().SkipLevel();

}


void InputManager::ManageMouseDown(const SDL_MouseButtonEvent& event)
{
    //std::cout << "MOUSE DOWN event: ";
    switch (event.button)
    {
    case SDL_BUTTON_LEFT:
    {
        if (auto it{ m_MouseDownMap.find(SDL_BUTTON_LEFT) }; it != m_MouseDownMap.end())
            it->second = true;

    } break;
    case SDL_BUTTON_RIGHT:
    {

        if (auto it{ m_MouseDownMap.find(SDL_BUTTON_RIGHT) }; it != m_MouseDownMap.end())
            it->second = true;

    } break;
    case SDL_BUTTON_MIDDLE:
    {

        if (auto it{ m_MouseDownMap.find(SDL_BUTTON_MIDDLE) }; it != m_MouseDownMap.end())
            it->second = true;

    } break;

    }


}


void InputManager::ManageMouseUp(const SDL_MouseButtonEvent& event)
{
    //std::cout << "MOUSEBUTTONUP event: ";

    switch (event.button)
    {
    case SDL_BUTTON_LEFT:
    {

        if (auto it{ m_MouseDownMap.find(SDL_BUTTON_LEFT) }; it != m_MouseDownMap.end())
            it->second = false;


    } break;
    case SDL_BUTTON_RIGHT:
    {

        if (auto it{ m_MouseDownMap.find(SDL_BUTTON_RIGHT) }; it != m_MouseDownMap.end())
            it->second = false;

    } break;
    case SDL_BUTTON_MIDDLE:
    {

        if (auto it{ m_MouseDownMap.find(SDL_BUTTON_MIDDLE) }; it != m_MouseDownMap.end())
            it->second = false;

    } break;

    }


}


void InputManager::ManageMouseMotion(const SDL_MouseMotionEvent& event)
{
}

Point2f InputManager::GetMousePosition()
{
    int x, y;
    SDL_GetMouseState(&x, &y);

    return Point2f(static_cast<float>(x), static_cast<float>(y));
}


