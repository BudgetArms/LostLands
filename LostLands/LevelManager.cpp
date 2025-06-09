#include "pch.h"
#include "LevelManager.h"

#include "Globals.h"
#include "Texture.h"
#include "Player.h"
#include "WinDoor.h"
#include "EntityManager.h"


void SetLevelTutorialDash();
void SetLevelTutorialShooting();
void SetLevelTutorialMirror();
void SetLevel1();
void SetLevel2();
void SetLevel3();


LevelManager::LevelManager() :
    m_uTextInfo{ std::make_unique<Texture>("InfoText Default", m_ResourcePath + std::string("Aovel.ttf"), 30, Color4f(1.f, 1.f, 1.f, 1.f)) },
    m_uTextWon{ std::make_unique<Texture>("Won Game, Press R to Restart", m_ResourcePath + std::string("Aovel.ttf"), 30, Color4f(1.f, 1.f, 1.f, 1.f)) },
    m_uTextDead{ std::make_unique<Texture>("You died, Press R to Restart", m_ResourcePath + std::string("Aovel.ttf"), 30, Color4f(1.f, 1.f, 1.f, 1.f)) },
    m_uTextMirror{ std::make_unique<Texture>("Mirror", m_ResourcePath + std::string("Aovel.ttf"), 26, Color4f(0.f, 0.f, 0.f, 1.f)) },
    m_uTextDash{ std::make_unique<Texture>("Dash", m_ResourcePath + std::string("Aovel.ttf"), 26, Color4f(0.f, 0.f, 0.f, 1.f)) }
{
    if (!m_uTextWon->IsCreationOk())
        std::abort();

    if (!m_uTextDead->IsCreationOk())
        std::abort();

    m_ArrLoadLevel.push_back(SetLevelTutorialDash);
    m_ArrLoadLevel.push_back(&SetLevelTutorialShooting);
    m_ArrLoadLevel.push_back(&SetLevelTutorialMirror);
    m_ArrLoadLevel.push_back(&SetLevel1);
    m_ArrLoadLevel.push_back(&SetLevel2);

    m_NrOfLevels = static_cast<int>(m_ArrLoadLevel.size());

    m_CurrentLevel = 0;
    m_CurrentLevel = 4;

}

LevelManager::~LevelManager() {};


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
    m_uTextDash->Draw(Point2f(30, g_Window.height - 30));
    m_uTextMirror->Draw(Point2f(210, g_Window.height - 30));


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
    if (auto* player = EntityManager::GetInstance().GetPlayer())
        if (player->IsDead())
            m_bLostGame = true;

    if (!m_bWonGame)
        if (HasPlayerFinishedLevel())
            SetLevel(++m_CurrentLevel);

}


void LevelManager::SkipLevel()
{
    if (m_CurrentLevel == m_NrOfLevels)
        m_CurrentLevel = 0;
    else
        ++m_CurrentLevel;

    ResetLevel();
}

void LevelManager::SetLevel(int level)
{
    std::cout << "Setting Level: " << level << "\n";
    EntityManager::GetInstance().Reset();
    m_CurrentLevel = level;

    m_bWonGame = false;
    m_bLostGame = false;

    if (m_CurrentLevel < 0 || m_CurrentLevel > m_NrOfLevels)
        std::abort();


    if (m_CurrentLevel == m_NrOfLevels)
    {
        std::cout << "WON GAME!!!\n";
        m_bWonGame = true;
        return;
    }

    // reset the current level
    m_uTextInfo = nullptr;
    m_ArrLoadLevel[m_CurrentLevel]();


}


void LevelManager::ResetLevel()
{
    if (m_bWonGame)
        m_CurrentLevel = 0;

    EntityManager::GetInstance().Reset();
    SetLevel(m_CurrentLevel);
}

bool LevelManager::HasPlayerFinishedLevel() const
{
    auto& entityManager = EntityManager::GetInstance();
    const auto& player = entityManager.GetPlayer();

    bool isOverlapping{ false };
    for (auto& uWinDoor : entityManager.GetWinDoors())
    {
        if (uWinDoor->IsActive())
        {
            if (utils::IsOverlapping(player->GetHitBox(), uWinDoor->GetArea()))
            {
                isOverlapping = true;
                break;
            }
        }
    }

    return isOverlapping;

}

void SetLevelTutorialDash()
{
    auto& entityManager = EntityManager::GetInstance();
    auto& levelManager = LevelManager::GetInstance();
    entityManager.Reset();

    levelManager.m_uTextInfo = std::make_unique<Texture>("Press Space to Dash (4 sec cooldown)", levelManager.GetResourcePath() + std::string("Aovel.ttf"), 30, Color4f(0.f, 0.f, 1.f, 1.f));

    auto player = entityManager.SpawnPlayer(Point2f(100, 250));
    player->SetShootingEnabled(false);
    player->SetDashingEnabled(true);
    player->SetMirroringEnabled(false);


    entityManager.SpawnSpeedPad(Point2f(400, 480), Vector2f(-1, 0), 600.f);
    entityManager.SpawnSpeedPad(Point2f(400, 390), Vector2f(-1, 0), 600.f);
    entityManager.SpawnSpeedPad(Point2f(400, 300), Vector2f(-1, 0), 600.f);
    entityManager.SpawnSpeedPad(Point2f(400, 210), Vector2f(-1, 0), 600.f);
    entityManager.SpawnSpeedPad(Point2f(400, 120), Vector2f(-1, 0), 600.f);
    entityManager.SpawnSpeedPad(Point2f(400, 30), Vector2f(-1, 0), 600.f);

    entityManager.SpawnWinDoor(Rectf(600, 210, 100, 100), true);
}

void SetLevelTutorialShooting()
{
    EntityManager& entityManager = EntityManager::GetInstance();
    auto& levelManager = LevelManager::GetInstance();
    entityManager.Reset();

    levelManager.m_uTextInfo = std::make_unique<Texture>("Press LMB to Shoot", levelManager.GetResourcePath() + std::string("Aovel.ttf"), 30, Color4f(0.f, 0.f, 1.f, 1.f));

    auto player = entityManager.SpawnPlayer(Point2f(100, 250));
    player->SetShootingEnabled(true);
    player->SetDashingEnabled(true);
    player->SetMirroringEnabled(false);

    entityManager.SpawnEnemy(Point2f(400, 400));
    entityManager.SpawnEnemy(Point2f(400, 400));
    entityManager.SpawnEnemy(Point2f(400, 300));
    entityManager.SpawnEnemy(Point2f(400, 200));
    entityManager.SpawnEnemy(Point2f(400, 100));

    entityManager.SpawnWinDoor(Rectf(600, 210, 100, 100), true);
}


void SetLevelTutorialMirror()
{
    EntityManager& entityManager = EntityManager::GetInstance();
    auto& levelManager = LevelManager::GetInstance();
    entityManager.Reset();

    levelManager.m_uTextInfo = std::make_unique<Texture>("Press RMB to Mirror (5 second cooldown)", levelManager.GetResourcePath() + std::string("Aovel.ttf"), 30, Color4f(0.f, 0.f, 1.f, 0.5f));

    auto player = entityManager.SpawnPlayer(Point2f(100, 250));
    player->SetShootingEnabled(true);
    player->SetDashingEnabled(true);
    player->SetMirroringEnabled(true);

    entityManager.SpawnWall(Rectf(400, 30, 80, 440));

    entityManager.SpawnWinDoor(Rectf(600, 210, 100, 100), false);
}



void SetLevel1()
{
    EntityManager& entityManager = EntityManager::GetInstance();
    auto& levelManager = LevelManager::GetInstance();
    entityManager.Reset();

    levelManager.m_uTextInfo = std::make_unique<Texture>("Level 1", levelManager.GetResourcePath() + std::string("Aovel.ttf"), 30, Color4f(0.f, 0.f, 1.f, 1.f));

    auto player = entityManager.SpawnPlayer(Point2f(100, 250));
    player->SetShootingEnabled(true);
    player->SetDashingEnabled(true);
    player->SetMirroringEnabled(true);

    entityManager.SpawnShootingEnemy(Point2f(490, 200));
    entityManager.SpawnShootingEnemy(Point2f(490, 240), 0.5f);

    entityManager.SpawnWinDoor(Rectf(600, 210, 100, 100), true);
}


void SetLevel2()
{
    EntityManager& entityManager = EntityManager::GetInstance();
    auto& levelManager = LevelManager::GetInstance();
    entityManager.Reset();

    levelManager.m_uTextInfo = std::make_unique<Texture>("Level 2 (mirror disabled)", levelManager.GetResourcePath() + std::string("Aovel.ttf"), 30, Color4f(0.f, 0.f, 1.f, 1.f));

    auto player = entityManager.SpawnPlayer(Point2f(80, 60));
    player->SetShootingEnabled(true);
    player->SetDashingEnabled(true);
    player->SetMirroringEnabled(false);
    player->SetBouncinessWalls(0.0);

    entityManager.SpawnWall(Rectf(30, 80, 200, 10));
    entityManager.SpawnWall(Rectf(600, 400, 230, 10));
    entityManager.SpawnDeadlyWall(Rectf(600, 400, 230, 10), 100);


    float width = 30;
    float height = 30;
    entityManager.SpawnWinDoor(Rectf(g_Window.width / 2 - width / 2, g_Window.height / 2 - height / 2,
        width, height), false);

}


