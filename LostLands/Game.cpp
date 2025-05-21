#include "pch.h"
#include "Game.h"

#include <filesystem>

#include "InputManager.h"
#include "EntityManager.h"
#include "LevelManager.h"

#include "utils.h"
#include "Globals.h"

#include "Player.h"
#include "Texture.h"



Game::Game(const Window& window) :
    BaseGame{ window },
    m_ResourcePath{ "" },
    m_uFpsTexture{ nullptr },
    m_AccumulatedSec{ 0.0f },
    m_FPS{ 0.0f }
{

    //m_uEntityManager->AddShootingEnemy(Point2f(300, 400));

    LevelManager::GetInstance().Start();

}

Game::~Game()
{
    std::cout << "Nothing to clean up" << "\n";
}



void Game::Update(float elapsedSec)
{

    m_AccumulatedSec += elapsedSec;

    LevelManager::GetInstance().Update(elapsedSec);
    EntityManager::GetInstance().Update(elapsedSec);
    InputManager::GetInstance().Update(elapsedSec);


    if (m_AccumulatedSec > 0.5f)
    {
        m_AccumulatedSec = 0;
        m_FPS = 1 / elapsedSec;

        std::string fpsText{ "FPS: " + std::to_string(int(m_FPS)) };

        if (m_uFpsTexture)
            m_uFpsTexture.reset();

        m_uFpsTexture = std::make_unique<Texture>(fpsText, m_ResourcePath + std::string("Aovel.ttf"), 20, Color4f(0.4f, 0.4f, 0.4f, 1.f));

        if (!m_uFpsTexture->IsCreationOk())
            std::abort();

    }


}

void Game::Draw() const
{


    ClearBackground();
    glClearColor(1.f, 1.f, 1.f, 1.f);


    utils::SetColor(1.f, 1.f, 1.f, 0.9f);
    utils::FillRect(g_Window);


    utils::SetColor(0.f, 0.f, 0.f, 1.f);
    utils::FillRect(g_SmallWindow);

    LevelManager::GetInstance().Draw();
    EntityManager::GetInstance().Draw();


    if (m_uFpsTexture)
        m_uFpsTexture->Draw(Point2f(700, 470));


}



void Game::ClearBackground() const
{
    glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}



#pragma region Key & Mouse Events



void Game::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{
    InputManager::GetInstance().ManageKeyDown(e);
}

void Game::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
    InputManager::GetInstance().ManageKeyUp(e);
}

void Game::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
    InputManager::GetInstance().ManageMouseMotion(e);
}

void Game::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
    InputManager::GetInstance().ManageMouseDown(e);
}

void Game::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{
    InputManager::GetInstance().ManageMouseUp(e);

}



#pragma endregion




