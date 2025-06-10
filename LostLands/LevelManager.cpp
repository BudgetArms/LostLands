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
    m_uTextLevel4{ std::make_unique<Texture>("WELL PLAYED", m_ResourcePath + std::string("Aovel.ttf"), 50, Color4f(1.f, 1.f, 1.f, 1.f)) },
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
        m_uTextInfo->Draw(Point2f(400, g_Window.height - 30));
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


    if (m_CurrentLevel == 4)
    {
        // to hide the secret mirror (even if you win the game)
        // nvm it blocks the player
        //utils::SetColor(0, 0, 0, 1.f);
        //utils::FillRect(Rectf(531, 42, 90, 55));


        m_uTextLevel4->Draw(Point2f(390, g_WindowOffset - 10));
        //utils::FillRect(g_WindowOffset, g_WindowOffset, g_SmallWindow.width, g_SmallWindow.height - 200);

        // hitlighting specia area
        utils::SetColor(1.1f, 1.f, 0.f, 0.2f);
        utils::FillRect(g_WindowOffset + 480, g_WindowOffset + 110, 120, 70);

        if (!m_bHasDiscoveredMiddleRight)
        {
            utils::SetColor(0, 0, 0.2f);
            utils::FillRect(g_WindowOffset + 370, g_WindowOffset + 100, 416, 250);
        }

        if (!m_bHasDiscoveredMiddleMirror)
        {
            utils::SetColor(0, 0, 0.2f);
            utils::FillRect(g_WindowOffset + 610, g_WindowOffset + 100, 80, 80);
        }

        if (!m_bHasDiscoveredMiddleCenter)
        {
            utils::SetColor(0, 0, 0.2f);
            utils::FillRect(g_WindowOffset + 70, g_WindowOffset + 170, 300, 180);
        }

        if (!m_bHasDiscoveredMiddleLeft)
        {
            utils::SetColor(0, 0, 0.2f);
            utils::FillRect(g_WindowOffset, g_WindowOffset + 100, 470, 70);
            utils::FillRect(g_WindowOffset, g_WindowOffset + 100, 70, 250);
        }


        if (!m_bHasDiscoveredBottom)
        {
            utils::SetColor(0, 0, 0.2f);
            utils::FillRect(g_WindowOffset, g_WindowOffset, g_SmallWindow.width, 100);
            utils::FillRect(g_WindowOffset + 700, g_WindowOffset, 86, 180);
        }


    }


}


void LevelManager::Update(float elapsedSec)
{
    if (auto* player = EntityManager::GetInstance().GetPlayer())
    {
        if (player)
        {
            if (player->IsDead())
                m_bLostGame = true;

            if (m_CurrentLevel == 4)
            {

                if (utils::IsPointInRect(player->m_Position, Rectf(g_WindowOffset + g_SmallWindow.width - 100, g_WindowOffset + g_SmallWindow.height - 100, 100, 10)))
                    m_bHasDiscoveredMiddleRight = true;

                if (utils::IsPointInRect(player->m_Position, Rectf(g_WindowOffset + 480, g_WindowOffset + 110, 120, 70)))
                    m_bHasDiscoveredMiddleMirror = true;

                if (utils::IsPointInRect(player->m_Position, Rectf(g_WindowOffset + 80, g_WindowOffset + 180, 90, 170)))
                    m_bHasDiscoveredMiddleCenter = true;

                if (utils::IsPointInRect(player->m_Position, Rectf(g_WindowOffset + 390, g_WindowOffset + 110, 77, 60)))
                    m_bHasDiscoveredMiddleLeft = true;


                if (utils::IsPointInRect(player->m_Position, Rectf(g_WindowOffset, g_WindowOffset, 786, 100)) ||
                    utils::IsPointInRect(player->m_Position, Rectf(g_WindowOffset + 700, g_WindowOffset, 86, 180)))
                {
                    m_bHasDiscoveredMiddleRight = true;
                    m_bHasDiscoveredMiddleMirror = true;
                    m_bHasDiscoveredMiddleCenter = true;
                    m_bHasDiscoveredMiddleLeft = true;
                    m_bHasDiscoveredBottom = true;
                }


            }

        }
        else
            std::cout << "wtf\n";

    }

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
    //
    m_bHasDiscoveredMiddleRight = false;
    m_bHasDiscoveredMiddleCenter = false;
    m_bHasDiscoveredMiddleMirror = false;
    m_bHasDiscoveredMiddleLeft = false;
    m_bHasDiscoveredBottom = false;


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

    levelManager.m_uTextInfo = std::make_unique<Texture>("Press Space to Dash",
        levelManager.GetResourcePath() + std::string("Aovel.ttf"), 26, Color4f(0.f, 0.f, 0.f, 1.f));

    auto player = entityManager.SpawnPlayer(Point2f(100, 250));
    player->m_bIsShootingEnabled = false;
    player->SetDashingEnabled(true);
    player->SetMirroringEnabled(false);


    entityManager.SpawnSpeedPad(Point2f(400, 480), Vector2f(-1, 0), 600.f);
    entityManager.SpawnSpeedPad(Point2f(400, 390), Vector2f(-1, 0), 600.f);
    entityManager.SpawnSpeedPad(Point2f(400, 300), Vector2f(-1, 0), 600.f);
    entityManager.SpawnSpeedPad(Point2f(400, 210), Vector2f(-1, 0), 600.f);
    entityManager.SpawnSpeedPad(Point2f(400, 120), Vector2f(-1, 0), 600.f);
    entityManager.SpawnSpeedPad(Point2f(400, 30), Vector2f(-1, 0), 600.f);

    entityManager.SpawnWinDoor(Rectf(650, 230, 70, 70), true);
}

void SetLevelTutorialShooting()
{
    EntityManager& entityManager = EntityManager::GetInstance();
    auto& levelManager = LevelManager::GetInstance();
    entityManager.Reset();

    levelManager.m_uTextInfo = std::make_unique<Texture>("Press LMB to Shoot",
        levelManager.GetResourcePath() + std::string("Aovel.ttf"), 26, Color4f(0.f, 0.f, 0.f, 1.f));

    auto player = entityManager.SpawnPlayer(Point2f(100, 250));
    player->m_bIsShootingEnabled = true;
    player->SetDashingEnabled(true);
    player->SetMirroringEnabled(false);

    entityManager.SpawnShootingEnemy(Point2f(400, 400), 0.5f);
    entityManager.SpawnEnemy(Point2f(400, 300));
    entityManager.SpawnEnemy(Point2f(400, 200));
    entityManager.SpawnShootingEnemy(Point2f(400, 100), 0.8f);

    entityManager.SpawnWinDoor(Rectf(600, 210, 100, 100), true);
}


void SetLevelTutorialMirror()
{
    EntityManager& entityManager = EntityManager::GetInstance();
    auto& levelManager = LevelManager::GetInstance();
    entityManager.Reset();

    levelManager.m_uTextInfo = std::make_unique<Texture>("Press RMB to Mirror",
        levelManager.GetResourcePath() + std::string("Aovel.ttf"), 26, Color4f(0.f, 0.f, 0.f, 1.f));

    auto player = entityManager.SpawnPlayer(Point2f(100, 250));
    player->m_bIsShootingEnabled = true;
    player->SetDashingEnabled(true);
    player->SetMirroringEnabled(true);

    entityManager.SpawnDeadlyWall(Rectf(g_WindowOffset + 370, g_WindowOffset, 80, 440), 50);

    entityManager.SpawnMirrorArea(Rectf(g_WindowOffset + 66, g_WindowOffset + 288, 150, 150));
    entityManager.SpawnMirrorArea(Rectf(g_WindowOffset + 570, g_WindowOffset, 150, 150));

    entityManager.SpawnWinDoor(Rectf(g_WindowOffset + 620, g_WindowOffset + 200, 70, 70), true);
}



void SetLevel1()
{
    EntityManager& entityManager = EntityManager::GetInstance();
    auto& levelManager = LevelManager::GetInstance();
    entityManager.Reset();

    levelManager.m_uTextInfo = std::make_unique<Texture>("Level 1",
        levelManager.GetResourcePath() + std::string("Aovel.ttf"), 26, Color4f(0.f, 0.f, 0.f, 1.f));

    auto player = entityManager.SpawnPlayer(Point2f(100, 250));
    player->m_bIsShootingEnabled = true;
    player->SetDashingEnabled(true);
    player->SetMirroringEnabled(true);

    entityManager.SpawnShootingEnemy(Point2f(490, 200));
    entityManager.SpawnShootingEnemy(Point2f(490, 280), 0.7f);

    entityManager.SpawnWinDoor(Rectf(600, 210, 100, 100), true);
}


void SetLevel2()
{
    EntityManager& entityManager = EntityManager::GetInstance();
    auto& levelManager = LevelManager::GetInstance();
    entityManager.Reset();

    levelManager.m_uTextInfo = std::make_unique<Texture>("Level 2",
        levelManager.GetResourcePath() + std::string("Aovel.ttf"), 26, Color4f(0.f, 0.f, 0.f, 1.f));


    auto player = entityManager.SpawnPlayer(Point2f(65, 420));
    //player = entityManager.SpawnPlayer(Point2f(765, 420)); // easier spawn
    player->m_bIsShootingEnabled = true;
    player->SetDashingEnabled(true);
    player->SetMirroringEnabled(true);
    player->SetBouncinessWalls(0.5f);
    player->m_DashTime = 0.5f;




    // Top Section
    entityManager.SpawnWall(Rectf(g_WindowOffset, g_WindowOffset + 350, 700, 10));
    entityManager.SpawnDeadlyWall(Rectf(100, 440, 120, 30), 100.f);
    entityManager.SpawnMovingDeadlyWall(Rectf(100, 430, 120, 30), 100.f, Vector2f(0, 1), 2.5f, 20);
    entityManager.SpawnMovingDeadlyWall(Rectf(100, 410, 120, 30), 100.f, Vector2f(0, 1), 2.5f, 40);

    entityManager.SpawnDeadlyWall(Rectf(320, 390, 100, 50), 100.f);
    entityManager.SpawnMovingDeadlyWall(Rectf(320, 405, 100, 50), 100.f, Vector2f(0, -1), 2.5f, 30);

    entityManager.SpawnMovingDeadlyWall(Rectf(520, 415, 50, 30), 100.f, Vector2f(0, 1), 2.f, 40);
    entityManager.SpawnMovingDeadlyWall(Rectf(650, 415, 50, 30), 100.f, Vector2f(0, -1), 2.f, 40);



    // Middle Right Section
    entityManager.SpawnWall(Rectf(720, 300, 10, 90));
    entityManager.SpawnWall(Rectf(720, 180, 10, 30));
    entityManager.SpawnWall(Rectf(720, 210, 97, 10));
    entityManager.SpawnWall(Rectf(720, 70, 10, 110));

    entityManager.SpawnDeadlyWall(Rectf(630, 130, 10, 170), 50.f);

    entityManager.SpawnWall(Rectf(460, 300, 115, 10));
    entityManager.SpawnWall(Rectf(400, 200, 10, 180));

    entityManager.SpawnShootingEnemy(Point2f(555, 250), 0.8f);
    entityManager.SpawnShootingEnemy(Point2f(495, 250), 0.6f);
    entityManager.SpawnShootingEnemy(Point2f(530, 175), 1.f);

    entityManager.SpawnMirrorArea(Rectf(660, 160, 40, 30));
    entityManager.SpawnMirrorArea(Rectf(140, 310, 40, 40));



    // Middle Left Section
    entityManager.SpawnWall(Rectf(100, 200, 410, 10));
    entityManager.SpawnWall(Rectf(100, 200, 10, 180));
    entityManager.SpawnWall(Rectf(500, 130, 10, 70));
    entityManager.SpawnWall(Rectf(65, 200, 200, 10));

    entityManager.SpawnSpeedPad(Point2f(200, 290), Vector2f(-1, 0), 600.f);
    entityManager.SpawnSpeedPad(Point2f(200, 200), Vector2f(-1, 0), 600.f);

    entityManager.SpawnSpeedPad(Point2f(200, 120), Vector2f(-1, -1), 600.f);
    entityManager.SpawnSpeedPad(Point2f(290, 120), Vector2f(-1, 1), 600.f);

    entityManager.SpawnMirrorArea(Rectf(350, 312, 42, 42));
    entityManager.SpawnMirrorArea(Rectf(454, 147, 42, 42));

    entityManager.SpawnEnemy(Point2f(75, 255));

    entityManager.SpawnMirrorArea(Rectf(37, 314, 55, 55));
    entityManager.SpawnMirrorArea(Rectf(754, 131, 55, 55));


    // Bottom Section
    entityManager.SpawnWall(Rectf(g_WindowOffset, g_WindowOffset + 100, 700, 10));

    entityManager.SpawnSpeedPad(Point2f(200, g_WindowOffset), Vector2f(1, 0), 400.f);
    entityManager.SpawnSpeedPad(Point2f(290, g_WindowOffset), Vector2f(-1, 0), 600.f);
    //entityManager.SpawnMirrorArea(Rectf(g_WindowOffset + 400, g_WindowOffset, 100, 50));

    // Hidden Mirror Area Skip
    entityManager.SpawnMirrorArea(Rectf(531, 42, 90, 55));


    // winDoor
    const float width{ 30 };
    const float height{ 30 };
    entityManager.SpawnWinDoor(Rectf(g_WindowOffset + 25, g_WindowOffset + 30, width, height), false);




}


