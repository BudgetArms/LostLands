#include "pch.h"
#include "LevelManager.h"

#include <memory>

#include "Globals.h"

#include "Texture.h"
#include "Level.h"

#include "Player.h"

#include "EntityManager.h"


void SetLevelTutorialDash();
void SetLevelTutorialShooting();
void SetLevelTutorialMirror();
void SetLevel1();
void SetLevel2();
void SetLevel3();


LevelManager::LevelManager() :
    m_ResourcePath{ "" },
    m_uTextInfo{ std::make_unique<Texture>("InfoText Default", m_ResourcePath + std::string("Aovel.ttf"), 30, Color4f(1.f, 1.f, 1.f, 1.f)) },
    m_uTextWon{ std::make_unique<Texture>("Won Game, Press R to Restart", m_ResourcePath + std::string("Aovel.ttf"), 30, Color4f(1.f, 1.f, 1.f, 1.f)) },
    m_uTextDead{ std::make_unique<Texture>("You died, Press R to Restart", m_ResourcePath + std::string("Aovel.ttf"), 30, Color4f(1.f, 1.f, 1.f, 1.f)) },
    m_bWonGame{ false },
    m_bLostGame{ false }
{
    std::cout << "created LevelManager" << "\n";

    if (m_uTextWon->IsCreationOk() == false)
        std::abort();

    if (m_uTextDead->IsCreationOk() == false)
        std::abort();

    m_ArrLoadLevel.push_back(SetLevelTutorialDash);
    m_ArrLoadLevel.push_back(&SetLevelTutorialShooting);
    m_ArrLoadLevel.push_back(&SetLevelTutorialMirror);
    m_ArrLoadLevel.push_back(&SetLevel1);

    m_NrOfLevels = static_cast<int>(m_ArrLoadLevel.size());
}

void LevelManager::Start()
{
    if (m_ArrLoadLevel.empty())
    {
        std::cout << "No levels are loaded\n";
        std::abort();
    }

    ResetLevel();

}


void LevelManager::Draw() const
{
    utils::SetColor(1, 1, 0, 0.4f);
    utils::FillRect(m_WinDoor);

    if (!m_bWonGame && !m_bLostGame)
    {
        utils::SetColor(1.f, 1.f, 1.f, 1.f);
        m_uTextInfo->Draw(Point2f(50, g_Window.height - 65));

    }

    if (m_bLostGame)
    {
        utils::SetColor(1.f, 1.f, 1.f, 1.f);
        m_uTextDead->Draw(Point2f((g_Window.width - m_uTextDead->GetWidth()) / 2.f,
            (g_Window.height - m_uTextDead->GetHeight()) / 2.f));
    }

    if (m_bWonGame && m_uTextWon)
    {
        utils::SetColor(1.f, 1.f, 1.f, 1.f);
        m_uTextWon->Draw(Point2f((g_Window.width - m_uTextWon->GetWidth()) / 2.f,
            (g_Window.height - m_uTextWon->GetHeight()) / 2.f));
    }

}


void LevelManager::Update(float elapsedSec)
{
    if (EntityManager::GetInstance().GetEnemies().empty())
    {
        m_WinDoor = Rectf(600, 210, 100, 100);
    }

    if (HasPlayerFinishedLevel() && !m_bWonGame)
        SetLevel(++m_NrCurrentLevel);

}


void LevelManager::SkipLevel()
{
    if (m_NrCurrentLevel == m_NrOfLevels)
        m_NrCurrentLevel = 0;
    else
        ++m_NrCurrentLevel;

    ResetLevel();

}

void LevelManager::SetLevel(int level)
{
    std::cout << "Setting Level: " << level << "\n";
    m_NrCurrentLevel = level;

    m_bWonGame = false;
    m_bLostGame = false;

    if (m_NrCurrentLevel < 0 || m_NrCurrentLevel > m_NrOfLevels)
        std::abort();

    m_WinDoor = {};

    if (m_NrCurrentLevel == m_NrOfLevels)
    {
        // won game??
        std::cout << "won game ig\n";
        m_bWonGame = true;
        return;
    }

    // reset the current level
    m_uTextInfo = nullptr;
    m_ArrLoadLevel[m_NrCurrentLevel]();


}


void LevelManager::ResetLevel()
{
    if (m_bWonGame)
        m_NrCurrentLevel = 0;

    SetLevel(m_NrCurrentLevel);
}

bool LevelManager::HasPlayerFinishedLevel() const
{
    const auto& player = EntityManager::GetInstance().GetPlayer();
    return utils::IsOverlapping(player->GetHitBox(), m_WinDoor);
}

void SetLevelTutorialDash()
{
    auto& entityManager = EntityManager::GetInstance();
    auto& levelManager = LevelManager::GetInstance();
    entityManager.Reset();

    levelManager.m_uTextInfo = std::make_unique<Texture>("Press Space to Dash (4 sec cooldown)", levelManager.GetResourcePath() + std::string("Aovel.ttf"), 30, Color4f(1.f, 1.f, 1.f, 1.f));

    entityManager.SpawnPlayer(Point2f(100, 250));
    entityManager.GetPlayer()->SetShootingEnabled(false);
    entityManager.GetPlayer()->SetDashingEnabled(true);
    entityManager.GetPlayer()->SetMirroringEnabled(false);

    entityManager.SpawnSpeedPad(Point2f(400, 480), Vector2f(-1, 0), 8.5f);
    entityManager.SpawnSpeedPad(Point2f(400, 390), Vector2f(-1, 0), 8.f);
    entityManager.SpawnSpeedPad(Point2f(400, 300), Vector2f(-1, 0), 8.f);
    entityManager.SpawnSpeedPad(Point2f(400, 210), Vector2f(-1, 0), 8.f);
    entityManager.SpawnSpeedPad(Point2f(400, 120), Vector2f(-1, 0), 8.f);
    entityManager.SpawnSpeedPad(Point2f(400, 30), Vector2f(-1, 0), 8.f);

}

void SetLevelTutorialShooting()
{
    EntityManager& entityManager = EntityManager::GetInstance();
    auto& levelManager = LevelManager::GetInstance();
    entityManager.Reset();

    levelManager.m_uTextInfo = std::make_unique<Texture>("Press LMB to Shoot", levelManager.GetResourcePath() + std::string("Aovel.ttf"), 30, Color4f(1.f, 1.f, 1.f, 1.f));

    entityManager.SpawnPlayer(Point2f(100, 250));
    entityManager.GetPlayer()->SetShootingEnabled(true);
    entityManager.GetPlayer()->SetDashingEnabled(true);
    entityManager.GetPlayer()->SetMirroringEnabled(false);

    entityManager.SpawnEnemy(Point2f(400, 400));
    entityManager.SpawnEnemy(Point2f(400, 400));
    entityManager.SpawnEnemy(Point2f(400, 300));
    entityManager.SpawnEnemy(Point2f(400, 200));
    entityManager.SpawnEnemy(Point2f(400, 100));

}


void SetLevelTutorialMirror()
{
    EntityManager& entityManager = EntityManager::GetInstance();
    auto& levelManager = LevelManager::GetInstance();
    entityManager.Reset();

    levelManager.m_uTextInfo = std::make_unique<Texture>("Press RMB to Mirror (5 second cooldown)", levelManager.GetResourcePath() + std::string("Aovel.ttf"), 30, Color4f(1.f, 1.f, 1.f, 0.5f));

    entityManager.SpawnPlayer(Point2f(100, 250));
    entityManager.GetPlayer()->SetShootingEnabled(true);
    entityManager.GetPlayer()->SetDashingEnabled(true);
    entityManager.GetPlayer()->SetMirroringEnabled(true);

    entityManager.SpawnWall(Rectf(400, 30, 80, 440));
}



void SetLevel1()
{
    EntityManager& entityManager = EntityManager::GetInstance();
    auto& levelManager = LevelManager::GetInstance();
    entityManager.Reset();

    levelManager.m_uTextInfo = std::make_unique<Texture>("InfoText Default", levelManager.GetResourcePath() + std::string("Aovel.ttf"), 30, Color4f(1.f, 1.f, 1.f, 1.f));

    entityManager.SpawnPlayer(Point2f(100, 250));
    entityManager.GetPlayer()->SetShootingEnabled(true);
    entityManager.GetPlayer()->SetDashingEnabled(true);
    entityManager.GetPlayer()->SetMirroringEnabled(true);

    //entityManager.Spawnw

}


