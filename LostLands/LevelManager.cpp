#include "pch.h"
#include "LevelManager.h"

#include "Globals.h"
#include "EntityManager.h"

#include "Texture.h"
#include "Player.h"
#include "WinDoor.h"
#include "CheckPoint.h"
#include "Wall.h"


void SetLevelTutorialDash();
void SetLevelTutorialShooting();
void SetLevelTutorialMirror();
void SetLevel3();
void SetLevel4();


LevelManager::LevelManager() :
	m_uTextInfo{ std::make_unique<Texture>("InfoText Default", m_FontPath, m_FontSizeMedium, Color4f(1.f, 1.f, 1.f, 1.f)) },
	m_uTextWon{ std::make_unique<Texture>("Won Game, Press R to Restart", m_FontPath, m_FontSizeMedium, Color4f(1.f, 1.f, 1.f, 1.f)) },
	m_uTextDead{ std::make_unique<Texture>("You died, Press R to Restart", m_FontPath, m_FontSizeMedium, Color4f(1.f, 1.f, 1.f, 1.f)) },
	m_uTextLevelName{ std::make_unique<Texture>("Level x", m_FontPath, m_FontSizeMedium, Color4f(0.f, 0.f, 0.f, 1.f)) },
	m_uTextLevel4{ std::make_unique<Texture>("Nice", m_FontPath, m_FontSizeBig, Color4f(1.f, 1.f, 1.f, 1.f)) },
	m_uTextLevel5{ std::make_unique<Texture>("WELL PLAYED", m_FontPath, m_FontSizeMedium - 2, Color4f(1.f, 1.f, 1.f, 1.f)) },
	m_uTextMirror{ std::make_unique<Texture>("Mirror", m_FontPath, m_FontSizeSmall, Color4f(0.f, 0.f, 0.f, 1.f)) },
	m_uTextDash{ std::make_unique<Texture>("Dash", m_FontPath, m_FontSizeSmall, Color4f(0.f, 0.f, 0.f, 1.f)) }
{
	if (!m_uTextWon->IsCreationOk())
		std::abort();

	if (!m_uTextDead->IsCreationOk())
		std::abort();

	m_ArrLoadLevel.push_back(SetLevelTutorialDash);
	m_ArrLoadLevel.push_back(&SetLevelTutorialShooting);
	m_ArrLoadLevel.push_back(&SetLevelTutorialMirror);
	m_ArrLoadLevel.push_back(&SetLevel3);
	m_ArrLoadLevel.push_back(&SetLevel4);

	m_NrOfLevels = static_cast<int>(m_ArrLoadLevel.size());

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

	m_uTextLevelName->Draw(Point2f(g_Window.width / 2.f - 100.f, -6.f));

	m_uTextDash->Draw(Point2f(30.f, g_Window.height - 30.f));
	m_uTextMirror->Draw(Point2f(210.f, g_Window.height - 30.f));

	if (m_CurrentLevel == 2)
		Level2DrawThings();

	if (m_CurrentLevel == 3)
		Level3DrawThings();

	if (m_CurrentLevel == 4)
		Level4DrawThings();


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




}


void LevelManager::Update(float elapsedSec)
{
	if (auto* player = EntityManager::GetInstance().GetPlayer())
	{
		if (player)
		{
			if (player->IsDead())
				m_bLostGame = true;

			if (m_CurrentLevel == 2)
			{

				if (utils::IsPointInRect(player->m_Position, Rectf(g_WindowOffset + 579, g_WindowOffset + 0, 206, 116)))
					m_bHasLvl2DiscoveredRightBottom = true;

				if (utils::IsPointInRect(player->m_Position, Rectf(g_WindowOffset + 127, g_WindowOffset + 0, 226, g_SmallWindow.height)))
				{
					m_bHasLvl2DiscoveredMiddleLeft = true;

					for (auto& uEnemy : EntityManager::GetInstance().GetEnemies())
						uEnemy->m_bIsShootingEnabled = true;

				}

				if (utils::IsPointInRect(player->m_Position, Rectf(g_WindowOffset + 363, g_WindowOffset + 0, 70, 48)))
					m_bHasLvl2DiscoveredMiddleRight = true;

				if (utils::IsPointInRect(player->m_Position, Rectf(g_WindowOffset + 433, g_WindowOffset + 390, 137, 50)))
					m_bHasLvl2DiscoveredRightTop = true;

			}

			if (m_CurrentLevel == 3)
			{

				if (utils::IsPointInRect(player->m_Position, Rectf(g_WindowOffset + g_SmallWindow.width - 100, g_WindowOffset + g_SmallWindow.height - 100, 100, 10)))
				{
					m_bHasDiscoveredMiddleRight = true;

					for (auto& uEnemy : EntityManager::GetInstance().GetEnemies())
						uEnemy->m_bIsShootingEnabled = true;


				}

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
			if (m_CurrentLevel == 4)
			{
				Rectf lvl5_1{ g_WindowOffset + 590, g_WindowOffset, 70, 70 };
				Rectf lvl5_2{ g_WindowOffset, g_WindowOffset, 70, 70 };
				Rectf lvl5_3{ g_WindowOffset + 500, g_WindowOffset + 110, 80, 50 };
				Rectf lvl5_4{ g_WindowOffset + 200, g_WindowOffset + 80, 41, 41 };
				Rectf lvl5_5{ g_WindowOffset + 534, g_WindowOffset + 370, 54, 54 };
				Rectf lvl5_6{ g_WindowOffset + 212, g_WindowOffset + 386, 88, 54 };


				if (!m_bHasDiscovered1)
				{
					if (utils::IsPointInRect(player->m_Position, lvl5_1))
					{
						m_bHasDiscovered1 = true;
						for (auto& uEnemy : EntityManager::GetInstance().GetEnemies())
						{
							if (utils::IsOverlapping(uEnemy->GetHitBox(), Rectf(g_WindowOffset + 590, g_WindowOffset, 400, g_SmallWindow.height)))
								uEnemy->m_bIsShootingEnabled = true;

						}

					}
				}

				if (utils::IsPointInRect(player->m_Position, lvl5_2))
					m_bHasDiscovered2 = true;

				if (!m_bHasDiscovered3)
				{
					if (utils::IsPointInRect(player->m_Position, lvl5_3))
					{
						m_bHasDiscovered3 = true;
						EntityManager::GetInstance().GetPlayer()->m_Velocity = {};

						for (auto& uEnemy : EntityManager::GetInstance().GetEnemies())
							uEnemy->m_bIsShootingEnabled = true;

					}
				}

				if (utils::IsPointInRect(player->m_Position, lvl5_4))
					m_bHasDiscovered4 = true;

				if (utils::IsPointInRect(player->m_Position, lvl5_5))
					m_bHasDiscovered5 = true;

				if (utils::IsPointInRect(player->m_Position, lvl5_6))
					m_bHasDiscovered6 = true;


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
	auto& em = EntityManager::GetInstance();
	em.Reset();
	m_CurrentLevel = level;
	m_CurrentCheckPoint = nullptr;

	const std::string levelNameText{ "Level " + std::to_string(m_CurrentLevel) };
	m_uTextLevelName = std::make_unique<Texture>(levelNameText, m_FontPath, m_FontSizeMedium, Color4f(0.f, 0.f, 0.f, 1.f));


	m_bWonGame = false;
	m_bLostGame = false;



	m_bHasLvl2DiscoveredRightBottom = false;
	m_bHasLvl2DiscoveredRightTop = false;
	m_bHasLvl2DiscoveredMiddleLeft = false;
	m_bHasLvl2DiscoveredMiddleRight = false;


	m_bHasDiscoveredMiddleRight = false;
	m_bHasDiscoveredMiddleCenter = false;
	m_bHasDiscoveredMiddleMirror = false;
	m_bHasDiscoveredMiddleLeft = false;
	m_bHasDiscoveredBottom = false;

	m_bHasDiscovered1 = false;
	m_bHasDiscovered2 = false;
	m_bHasDiscovered3 = false;
	m_bHasDiscovered4 = false;
	m_bHasDiscovered5 = false;
	m_bHasDiscovered6 = false;


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
	Wall* pWall;

	// Left Wall
	pWall = em.SpawnWall(Rectf(0, 0, g_WindowOffset, g_Window.height));
	pWall->m_bRenderWall = false;

	// Top Wall
	pWall = em.SpawnWall(Rectf(0, g_Window.height - g_WindowOffset, g_Window.width, g_WindowOffset));
	pWall->m_bRenderWall = false;

	// Right Wall
	pWall = em.SpawnWall(Rectf(g_Window.width - g_WindowOffset, 0, g_WindowOffset, g_Window.height));
	pWall->m_bRenderWall = false;

	// Bottom Wall
	pWall = em.SpawnWall(Rectf(0, 0, g_Window.width, g_WindowOffset));
	pWall->m_bRenderWall = false;


}


void LevelManager::ResetLevel()
{
	m_CurrentCheckPoint = nullptr;

	if (m_bWonGame)
		m_CurrentLevel = 0;

	EntityManager::GetInstance().Reset();
	SetLevel(m_CurrentLevel);
}

bool LevelManager::HasPlayerFinishedLevel() const
{
	auto& em = EntityManager::GetInstance();
	const auto& player = em.GetPlayer();

	bool isOverlapping{ false };
	for (auto& uWinDoor : em.GetWinDoors())
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



void LevelManager::Level2DrawThings() const
{
	if (m_CurrentLevel != 2)
		return;

	utils::SetColor(m_HiddenColor);

	if (!m_bHasLvl2DiscoveredRightBottom)
	{
		utils::FillRect(g_WindowOffset + 127, g_WindowOffset + 0, g_SmallWindow.width - 127, g_SmallWindow.height);
	}

	if (!m_bHasLvl2DiscoveredMiddleLeft)
	{
		utils::FillRect(g_WindowOffset + 127, g_WindowOffset + 0, 307, g_SmallWindow.height);
	}

	if (!m_bHasLvl2DiscoveredMiddleRight)
	{
		utils::FillRect(g_WindowOffset + 433, g_WindowOffset + 0, 137, g_SmallWindow.height);
		utils::FillRect(g_WindowOffset + 679, g_WindowOffset + 126, 107, 314);
		utils::FillRect(g_WindowOffset + 570, g_WindowOffset + 296, 10, 144);
	}

	if (!m_bHasLvl2DiscoveredRightTop)
	{
		utils::FillRect(g_WindowOffset + 433, g_WindowOffset + 390, 353, 50);
		utils::FillRect(g_WindowOffset + 580, g_WindowOffset + 296, 206, 144);
		utils::FillRect(g_WindowOffset + 679, g_WindowOffset + 126, 107, 314);

	}

}

void LevelManager::Level3DrawThings() const
{
	if (m_CurrentLevel != 3)
		return;

	// to hide the secret mirror (even if you win the game)
	// nvm it blocks the player
	//utils::SetColor(0, 0, 0, 1.f);
	//utils::FillRect(Rectf(531, 42, 90, 55));


	m_uTextLevel4->Draw(Point2f(390, g_WindowOffset - 10));

	utils::SetColor(m_HiddenColor);

	if (!m_bHasDiscoveredMiddleRight)
		utils::FillRect(g_WindowOffset + 370, g_WindowOffset + 100, 416, 250);

	if (!m_bHasDiscoveredMiddleMirror)
		utils::FillRect(g_WindowOffset + 610, g_WindowOffset + 100, 80, 80);

	if (!m_bHasDiscoveredMiddleCenter)
		utils::FillRect(g_WindowOffset + 70, g_WindowOffset + 170, 300, 180);

	if (!m_bHasDiscoveredMiddleLeft)
	{
		utils::FillRect(g_WindowOffset, g_WindowOffset + 100, 470, 70);
		utils::FillRect(g_WindowOffset, g_WindowOffset + 100, 70, 250);
	}


	if (!m_bHasDiscoveredBottom)
	{
		utils::FillRect(g_WindowOffset, g_WindowOffset, g_SmallWindow.width, 100);
		utils::FillRect(g_WindowOffset + 700, g_WindowOffset, 86, 180);
	}



}

void LevelManager::Level4DrawThings() const
{
	if (m_CurrentLevel != 4)
		return;

	m_uTextLevel5->Draw(Point2f(160, 354));


	utils::SetColor(m_HiddenColor);

	//return;
	if (!m_bHasDiscovered1)
	{

		std::vector<Point2f> BigArea =
		{
			Point2f(g_WindowOffset, g_WindowOffset),
			Point2f(g_WindowOffset, g_Window.height - g_WindowOffset - 54),
			Point2f(g_WindowOffset + 212, g_Window.height - g_WindowOffset - 54),
			Point2f(g_Window.width - g_WindowOffset, g_Window.height - g_WindowOffset),
			Point2f(g_Window.width - g_WindowOffset, g_WindowOffset),

		};

		std::vector<Point2f> BigArea2 =
		{
			Point2f(g_WindowOffset + 212, g_Window.height - g_WindowOffset - 54),
			Point2f(g_WindowOffset + 212, g_Window.height - g_WindowOffset),
			Point2f(g_Window.width - g_WindowOffset - 198, g_Window.height - g_WindowOffset),
			Point2f(g_WindowOffset + 212, g_Window.height - g_WindowOffset - 54),
		};


		utils::FillPolygon(BigArea);
		utils::FillPolygon(BigArea2);

		utils::FillRect(Rectf(g_Window.width - g_WindowOffset - 198, g_Window.height - g_WindowOffset - 96, 198, 96));

	}


	if (!m_bHasDiscovered2)
	{

		std::vector<Point2f> BigArea =
		{
			Point2f(g_WindowOffset, g_WindowOffset + 70),
			Point2f(g_WindowOffset, g_Window.height - g_WindowOffset - 54),
			Point2f(g_WindowOffset + 212, g_Window.height - g_WindowOffset - 54),
			Point2f(g_Window.width - g_WindowOffset - 198, g_Window.height - g_WindowOffset),
			Point2f(g_Window.width - g_WindowOffset - 198, g_WindowOffset + 70),
			Point2f(g_Window.width - g_WindowOffset - 198 - 88, g_WindowOffset + 70),
			Point2f(g_Window.width - g_WindowOffset - 198 - 88, g_WindowOffset + 70),
			Point2f(g_Window.width - g_WindowOffset - 198 - 88, g_WindowOffset),

		};


		std::vector<Point2f> BigArea2 =
		{
			Point2f(g_WindowOffset + 212, g_Window.height - g_WindowOffset - 54),
			Point2f(g_WindowOffset + 212, g_Window.height - g_WindowOffset),
			Point2f(g_Window.width - g_WindowOffset - 198, g_Window.height - g_WindowOffset),
			Point2f(g_WindowOffset + 212, g_Window.height - g_WindowOffset - 54),
		};

		std::vector<Point2f> BigArea3 =
		{
			Point2f(g_WindowOffset, g_WindowOffset),
			Point2f(g_WindowOffset, g_WindowOffset + 70),
			Point2f(g_Window.width - g_WindowOffset - 198 - 88, g_WindowOffset + 70),
			Point2f(g_Window.width - g_WindowOffset - 198 - 88, g_WindowOffset),
		};

		utils::FillPolygon(BigArea);
		utils::FillPolygon(BigArea2);
		utils::FillPolygon(BigArea3);

	}

	if (!m_bHasDiscovered3)
	{
		std::vector<Point2f> BigArea =
		{
			Point2f(g_WindowOffset, g_WindowOffset + 161),
			Point2f(g_WindowOffset, g_Window.height - g_WindowOffset - 54),
			Point2f(g_WindowOffset + 212, g_Window.height - g_WindowOffset - 54),
			Point2f(g_Window.width - g_WindowOffset - 198, g_Window.height - g_WindowOffset),
			Point2f(g_Window.width - g_WindowOffset - 198, g_WindowOffset + 161),
			Point2f(g_Window.width - g_WindowOffset - 198 - 88, g_WindowOffset + 161),
			Point2f(g_Window.width - g_WindowOffset - 198 - 88, g_WindowOffset + 161),

		};

		std::vector<Point2f> BigArea2 =
		{
			Point2f(g_WindowOffset + 212, g_Window.height - g_WindowOffset - 54),
			Point2f(g_WindowOffset + 212, g_Window.height - g_WindowOffset),
			Point2f(g_Window.width - g_WindowOffset - 198, g_Window.height - g_WindowOffset),
			Point2f(g_WindowOffset + 212, g_Window.height - g_WindowOffset - 54),
		};

		std::vector<Point2f> BigArea3 =
		{
			Point2f(g_WindowOffset, g_WindowOffset + 80),
			Point2f(g_WindowOffset, g_WindowOffset + 80 + 82),
			Point2f(g_WindowOffset + 480, g_WindowOffset + 80 + 82),
			Point2f(g_WindowOffset + 480, g_WindowOffset + 80),
		};

		utils::FillPolygon(BigArea);
		utils::FillPolygon(BigArea2);
		utils::FillPolygon(BigArea3);

	}

	if (!m_bHasDiscovered4)
	{
		std::vector<Point2f> BigArea =
		{
			Point2f(g_WindowOffset, g_WindowOffset + 80),
			Point2f(g_WindowOffset, g_Window.height - g_WindowOffset - 54),
			Point2f(g_WindowOffset + 240, g_Window.height - g_WindowOffset - 54),
			Point2f(g_WindowOffset + 240, g_WindowOffset + 80),
		};

		std::vector<Point2f> BigArea2 =
		{
			Point2f(g_WindowOffset + 213, g_WindowOffset + 318),
			Point2f(g_WindowOffset + 213, g_Window.height - g_WindowOffset),
			Point2f(g_Window.width - g_WindowOffset - 252, g_Window.height - g_WindowOffset),
			Point2f(g_Window.width - g_WindowOffset - 252, g_Window.height - g_WindowOffset),
			Point2f(g_Window.width - g_WindowOffset - 252, g_WindowOffset + 318),
		};

		std::vector<Point2f> BigArea3 =
		{
			Point2f(g_Window.width - g_WindowOffset - 252, g_Window.height - g_WindowOffset - 70),
			Point2f(g_Window.width - g_WindowOffset - 252, g_Window.height - g_WindowOffset),
			Point2f(g_Window.width - g_WindowOffset - 198, g_Window.height - g_WindowOffset),
			Point2f(g_Window.width - g_WindowOffset - 198, g_Window.height - g_WindowOffset - 70),
		};

		utils::FillPolygon(BigArea);
		utils::FillPolygon(BigArea2);
		utils::FillPolygon(BigArea3);

	}

	if (!m_bHasDiscovered5)
	{

		std::vector<Point2f> BigArea =
		{
			Point2f(g_WindowOffset + 124, g_WindowOffset + 144),
			Point2f(g_WindowOffset + 124, g_Window.height - g_WindowOffset - 54),
			Point2f(g_WindowOffset + 240, g_Window.height - g_WindowOffset - 54),
			Point2f(g_WindowOffset + 240, g_WindowOffset + 144),
		};

		std::vector<Point2f> BigArea2 =
		{
			Point2f(g_WindowOffset + 213, g_WindowOffset + 318),
			Point2f(g_WindowOffset + 213, g_Window.height - g_WindowOffset),
			Point2f(g_Window.width - g_WindowOffset - 252, g_Window.height - g_WindowOffset),
			Point2f(g_Window.width - g_WindowOffset - 252, g_Window.height - g_WindowOffset),
			Point2f(g_Window.width - g_WindowOffset - 252, g_WindowOffset + 318),
		};

		std::vector<Point2f> BigArea3 =
		{
			Point2f(g_Window.width - g_WindowOffset - 252, g_Window.height - g_WindowOffset - 70),
			Point2f(g_Window.width - g_WindowOffset - 252, g_Window.height - g_WindowOffset),
			Point2f(g_Window.width - g_WindowOffset - 198, g_Window.height - g_WindowOffset),
			Point2f(g_Window.width - g_WindowOffset - 198, g_Window.height - g_WindowOffset - 70),
		};

		utils::FillPolygon(BigArea);
		utils::FillPolygon(BigArea2);
		utils::FillPolygon(BigArea3);

	}

	if (!m_bHasDiscovered6)
	{

		std::vector<Point2f> BigArea =
		{
			Point2f(g_WindowOffset + 124, g_WindowOffset + 144),
			Point2f(g_WindowOffset + 124, g_Window.height - g_WindowOffset - 54),
			Point2f(g_WindowOffset + 240, g_Window.height - g_WindowOffset - 54),
			Point2f(g_WindowOffset + 240, g_WindowOffset + 144),
		};

		std::vector<Point2f> BigArea2 =
		{
			Point2f(g_WindowOffset + 240, g_Window.height - g_WindowOffset - 122),
			Point2f(g_WindowOffset + 240, g_Window.height - g_WindowOffset - 54),
			Point2f(g_WindowOffset + 240 + 60, g_Window.height - g_WindowOffset - 54),
			Point2f(g_WindowOffset + 240 + 60, g_Window.height - g_WindowOffset - 122),
		};


		utils::FillPolygon(BigArea);
		utils::FillPolygon(BigArea2);

	}



}


void SetLevelTutorialDash()
{
	auto& em = EntityManager::GetInstance();
	auto& levelManager = LevelManager::GetInstance();
	em.Reset();

	levelManager.GetTextInfo() = std::make_unique<Texture>("Press Space to Dash",
		levelManager.m_FontPath, 26, Color4f(0.f, 0.f, 0.f, 1.f));

	levelManager.m_CurrentSpawnPosition = Point2f(100, 250);

	auto player = em.SpawnPlayer(levelManager.m_CurrentSpawnPosition, 1);
	player->m_bIsShootingEnabled = false;
	player->SetDashingEnabled(true);
	player->SetMirroringEnabled(false);


	em.SpawnSpeedPad(Rectf(400, 390, 90, 80), Vector2f(-1, 0), 500.f);
	em.SpawnSpeedPad(Point2f(400, 300), Vector2f(-1, 0), 500.f);
	em.SpawnSpeedPad(Point2f(400, 210), Vector2f(-1, 0), 500.f);
	em.SpawnSpeedPad(Point2f(400, 120), Vector2f(-1, 0), 500.f);
	em.SpawnSpeedPad(Point2f(400, 30), Vector2f(-1, 0), 500.f);

	em.SpawnWinDoor(Rectf(650, 230, 70, 70), true);
}

void SetLevelTutorialShooting()
{
	EntityManager& em = EntityManager::GetInstance();
	auto& levelManager = LevelManager::GetInstance();
	em.Reset();

	levelManager.GetTextInfo() = std::make_unique<Texture>("Press LMB to Shoot",
		levelManager.m_FontPath, 26, Color4f(0.f, 0.f, 0.f, 1.f));

	levelManager.m_CurrentSpawnPosition = Point2f(100, 250);

	auto player = em.SpawnPlayer(levelManager.m_CurrentSpawnPosition, 2);
	player->m_bIsShootingEnabled = true;
	player->SetDashingEnabled(true);
	player->SetMirroringEnabled(false);


	em.SpawnWall(Rectf(g_WindowOffset + 300, g_WindowOffset + 100, 10, 100));
	em.SpawnWall(Rectf(g_WindowOffset + 200, g_WindowOffset + 250, 10, 100));

	em.SpawnEnemy(Point2f(g_WindowOffset + 370, g_WindowOffset + 170));
	em.SpawnShootingEnemy(Point2f(g_WindowOffset + 370, g_WindowOffset + 370), 0.5f);
	em.SpawnTurretEnemy(Point2f(g_WindowOffset + 400, g_WindowOffset + 70), Vector2f(-0.8f, -1), 1.f);


	// WinDoor
	em.SpawnWinDoor(Rectf(g_WindowOffset + 570, g_WindowOffset + 180, 100, 100), true);
}

void SetLevelTutorialMirror()
{
	EntityManager& em = EntityManager::GetInstance();
	auto& levelManager = LevelManager::GetInstance();
	em.Reset();

	levelManager.GetTextInfo() = std::make_unique<Texture>("Press RMB to Mirror",
		levelManager.m_FontPath, 26, Color4f(0.f, 0.f, 0.f, 1.f));

	levelManager.m_CurrentSpawnPosition = Point2f(g_WindowOffset + 58, g_WindowOffset + 80);
	//levelManager.m_CurrentSpawnPosition = Point2f(g_WindowOffset + 450, g_WindowOffset + 20);
	//levelManager.m_CurrentSpawnPosition = Point2f(g_WindowOffset + 720, g_WindowOffset + 350);

	auto player = em.SpawnPlayer(levelManager.m_CurrentSpawnPosition, 3);
	player->m_bIsShootingEnabled = true;
	player->SetDashingEnabled(true);
	player->SetMirroringEnabled(true);
	player->SetBouncinessWalls(0.1f);

	static float wallThickness{ 10.f };

	// Left part
	em.SpawnMirrorArea(Rectf(g_WindowOffset + 28, g_WindowOffset + 352, 60, 60));
	em.SpawnWall(Rectf(g_WindowOffset + 117, g_WindowOffset + 0, wallThickness, g_SmallWindow.height));


	// Left Middle Part
	em.SpawnMirrorArea(Rectf(g_WindowOffset + 134, g_WindowOffset + 181, 55, 55));

	em.SpawnWall(Rectf(g_WindowOffset + 117, g_WindowOffset + 90, 115, wallThickness));
	em.SpawnWall(Rectf(g_WindowOffset + 117, g_WindowOffset + 160, 143, wallThickness));
	em.SpawnWall(Rectf(g_WindowOffset + 117, g_WindowOffset + 247, 138, wallThickness));

	em.SpawnDeadlyWall(Rectf(g_WindowOffset + 353, g_WindowOffset + 48, 80, 344), 100.f);
	em.SpawnDeadlyWall(Rectf(g_WindowOffset + 353, g_WindowOffset + 0, wallThickness, 48), 100.f);
	em.SpawnDeadlyWall(Rectf(g_WindowOffset + 423, g_WindowOffset + g_SmallWindow.height - 48, wallThickness, 48), 100.f);

	auto pTopUnlockableWall = em.SpawnUnlockWall(Rectf(g_WindowOffset + 353, g_WindowOffset + g_SmallWindow.height - 48, wallThickness, 48));
	auto pBottomUnlockableWall = em.SpawnUnlockWall(Rectf(g_WindowOffset + 423, g_WindowOffset + 0, wallThickness, 48));

	em.SpawnUnlockArea(Rectf(g_WindowOffset + 143, g_WindowOffset + 380, 45, 45), pTopUnlockableWall);
	em.SpawnUnlockArea(Rectf(g_WindowOffset + 143, g_WindowOffset + 13, 62, 62), pBottomUnlockableWall);

	em.SpawnMirrorArea(Rectf(g_WindowOffset + 374, g_WindowOffset + 6, 37, 37));
	em.SpawnMirrorArea(Rectf(g_WindowOffset + 374, g_WindowOffset + g_SmallWindow.height - 37 - 6, 37, 37));

	em.SpawnTurretEnemy(Point2f(g_WindowOffset + 203, g_WindowOffset + 38), Vector2f(1, 1), 2.f);
	em.SpawnTurretEnemy(Point2f(g_WindowOffset + 176, g_WindowOffset + 296), Vector2f(1, 0), 1.5f);
	em.SpawnTurretEnemy(Point2f(g_WindowOffset + 203, g_WindowOffset + 370), Vector2f(1, -1), 2.4f);

	em.SpawnShootingEnemy(Point2f(g_WindowOffset + 161, g_WindowOffset + 131), 1.f);



	// Right Middle
	em.SpawnWall(Rectf(g_WindowOffset + 570, g_WindowOffset + 0, wallThickness, g_SmallWindow.height - 38));

	em.SpawnSpeedPad(Rectf(g_WindowOffset + 433, g_WindowOffset + 80, 137, 100), Vector2f(0, -1), 430);
	em.SpawnSpeedPad(Rectf(g_WindowOffset + 433, g_WindowOffset + 260, 137, 100), Vector2f(-1, 0), 375);

	em.SpawnCheckPoint(Rectf(g_WindowOffset + 470, g_WindowOffset + 8, 60, 60));


	// Right Bottom
	em.SpawnMirrorArea(Rectf(g_WindowOffset + 698, g_WindowOffset + 28, 60, 60));
	em.SpawnMirrorArea(Rectf(g_WindowOffset + 597, g_WindowOffset + 204, 55, 55));

	em.SpawnCheckPoint(Rectf(g_WindowOffset + 596, g_WindowOffset + 28, 60, 60));


	// Right Top
	em.SpawnWall(Rectf(g_WindowOffset + 570, g_WindowOffset + 0, wallThickness, g_SmallWindow.height - 38));
	em.SpawnWall(Rectf(g_WindowOffset + 669, g_WindowOffset + 116, wallThickness, 257));
	em.SpawnWall(Rectf(g_WindowOffset + 669, g_WindowOffset + 116, 117, wallThickness));

	em.SpawnDeadlyWall(Rectf(g_WindowOffset + 580, g_WindowOffset + 286, 89, wallThickness), 100);

	em.SpawnSpeedPad(Rectf(g_WindowOffset + 580, g_WindowOffset + 296, 89, 144), Vector2f(0, -1), 500);



	// WinDoor
	em.SpawnWinDoor(Rectf(g_WindowOffset + 698, g_WindowOffset + 143, 70, 70), true);


	for (auto& uEnemy : EntityManager::GetInstance().GetEnemies())
		uEnemy->m_bIsShootingEnabled = false;


}


void SetLevel3()
{
	EntityManager& em = EntityManager::GetInstance();
	auto& levelManager = LevelManager::GetInstance();
	em.Reset();

	levelManager.GetTextInfo() = std::make_unique<Texture>(" ",
		levelManager.m_FontPath, 26, Color4f(0.f, 0.f, 0.f, 1.f));

	levelManager.m_CurrentSpawnPosition = Point2f(65, 420);
	//levelManager.m_CurrentSpawnPosition = Point2f(765, 420); // Easy Spawn, skips top section

	auto player = em.SpawnPlayer(levelManager.m_CurrentSpawnPosition, 4);
	player->m_bIsShootingEnabled = true;
	player->SetDashingEnabled(true);
	player->SetMirroringEnabled(true);
	player->SetBouncinessWalls(0.5f);
	player->m_DashTime = 0.5f;


	// Top Section
	em.SpawnWall(Rectf(g_WindowOffset, g_WindowOffset + 350, 700, 10));
	em.SpawnDeadlyWall(Rectf(125, 440, 150, 30), 100.f);
	em.SpawnMovingDeadlyWall(Rectf(125, 410, 150, 30), 100.f, Vector2f(0, 1), 2.5f, 40);
	em.SpawnMovingDeadlyWall(Rectf(125, 430, 150, 30), 100.f, Vector2f(0, 1), 2.5f, 20);

	em.SpawnDeadlyWall(Rectf(400, 390, 125, 45), 100.f);
	em.SpawnMovingDeadlyWall(Rectf(400, 410, 125, 40), 100.f, Vector2f(0, -1), 2.f, 35);

	//em.SpawnMovingDeadlyWall(Rectf(550, 415, 50, 30), 100.f, Vector2f(0, 1), 2.f, 40);
	em.SpawnMovingDeadlyWall(Rectf(650, 415, 50, 30), 100.f, Vector2f(0, -1), 2.5f, 40);

	em.SpawnCheckPoint(Rectf(735, 250, 75, 75));



	// Middle Right Section
	em.SpawnWall(Rectf(720, 300, 10, 90));
	em.SpawnWall(Rectf(720, 180, 10, 30));
	em.SpawnWall(Rectf(720, 210, 97, 10));
	em.SpawnWall(Rectf(720, 70, 10, 110));

	em.SpawnDeadlyWall(Rectf(630, 130, 10, 170), 50.f);

	em.SpawnWall(Rectf(460, 300, 115, 10));
	em.SpawnWall(Rectf(400, 200, 10, 180));

	em.SpawnShootingEnemy(Point2f(555, 250), 0.8f);
	em.SpawnShootingEnemy(Point2f(495, 250), 0.6f);
	em.SpawnShootingEnemy(Point2f(530, 175), 1.f);

	em.SpawnMirrorArea(Rectf(660, 160, 40, 30));
	em.SpawnMirrorArea(Rectf(140, 310, 40, 40));


	auto pWall = em.SpawnUnlockWall(Rectf(g_WindowOffset + 610, g_WindowOffset + 180, 80, 10));
	em.SpawnUnlockArea(Rectf(g_WindowOffset + 480, g_WindowOffset + 110, 120, 70), pWall);


	// Middle Left Section
	em.SpawnWall(Rectf(100, 200, 410, 10));
	em.SpawnWall(Rectf(100, 200, 10, 180));
	em.SpawnWall(Rectf(500, 130, 10, 70));
	em.SpawnWall(Rectf(65, 200, 200, 10));

	em.SpawnSpeedPad(Point2f(200, 290), Vector2f(-1, 0), 600.f);
	em.SpawnSpeedPad(Point2f(200, 200), Vector2f(-1, 0), 600.f);

	em.SpawnSpeedPad(Point2f(200, 120), Vector2f(-1, -1), 600.f);
	em.SpawnSpeedPad(Point2f(290, 120), Vector2f(-1, 1), 600.f);

	em.SpawnMirrorArea(Rectf(350, 312, 42, 42));
	em.SpawnMirrorArea(Rectf(454, 147, 42, 42));

	em.SpawnEnemy(Point2f(75, 255));

	em.SpawnMirrorArea(Rectf(37, 314, 55, 55));
	em.SpawnMirrorArea(Rectf(754, 131, 55, 55));


	// Bottom Section
	em.SpawnWall(Rectf(g_WindowOffset, g_WindowOffset + 100, 700, 10));

	em.SpawnSpeedPad(Point2f(200, g_WindowOffset), Vector2f(1, 0), 400.f);
	em.SpawnSpeedPad(Point2f(290, g_WindowOffset), Vector2f(-1, 0), 600.f);
	//em.SpawnMirrorArea(Rectf(g_WindowOffset + 400, g_WindowOffset, 100, 50));

	// Hidden Mirror Area Skip
	em.SpawnMirrorArea(Rectf(531, 42, 90, 55));


	// winDoor
	const float width{ 30 };
	const float height{ 30 };
	em.SpawnWinDoor(Rectf(g_WindowOffset + 25, g_WindowOffset + 30, width, height), false);


	for (auto& uEnemy : em.GetEnemies())
		uEnemy->m_bIsShootingEnabled = false;

}


void SetLevel4()
{
	EntityManager& em = EntityManager::GetInstance();
	auto& levelManager = LevelManager::GetInstance();
	em.Reset();

	levelManager.GetTextInfo() = std::make_unique<Texture>(" ",
		levelManager.m_FontPath, 26, Color4f(0.f, 0.f, 0.f, 1.f));

	levelManager.m_CurrentSpawnPosition = Point2f(g_WindowOffset + 35.f, g_WindowOffset + 414.f);
	//levelManager.m_CurrentSpawnPosition = Point2f(g_WindowOffset + 35.f, g_WindowOffset + 20.f);
	//levelManager.m_CurrentSpawnPosition = Point2f(g_WindowOffset + 150.f, g_WindowOffset + 95.f);

	auto player = em.SpawnPlayer(levelManager.m_CurrentSpawnPosition, 5);
	player->m_bIsShootingEnabled = true;
	player->SetDashingEnabled(true);
	player->SetMirroringEnabled(true);
	player->SetBouncinessWalls(0.5f);
	player->m_DashTime = 0.5f;
	player->SetDashCoolDown(1.f);


	// Mirrors
	const float mirrorAreaSize{ 40.f };
	em.SpawnMirrorArea(Rectf(g_WindowOffset + 153, g_Window.height - g_WindowOffset - mirrorAreaSize - 2, mirrorAreaSize, mirrorAreaSize));
	em.SpawnMirrorArea(Rectf(g_Window.width - g_WindowOffset - mirrorAreaSize - 153, g_WindowOffset + 2, mirrorAreaSize, mirrorAreaSize));

	em.SpawnMirrorArea(Rectf(g_WindowOffset + 30, g_WindowOffset + 15, mirrorAreaSize, mirrorAreaSize));
	em.SpawnMirrorArea(Rectf(g_Window.width - g_WindowOffset - mirrorAreaSize - 30, g_Window.height - g_WindowOffset - mirrorAreaSize - 15, mirrorAreaSize, mirrorAreaSize));


	// Enemies


	// Right
	em.SpawnTurretEnemy(Point2f(g_WindowOffset + 690, g_WindowOffset + 199), Vector2f(-1, -1), 0.2f);
	em.SpawnShootingEnemy(Point2f(g_WindowOffset + 650, g_WindowOffset + 280), 0.4f);
	em.SpawnShootingEnemy(Point2f(g_WindowOffset + 630, g_WindowOffset + 400), 0.4f);


	// Middle Middle
	em.SpawnTurretEnemy(Point2f(g_WindowOffset + 289, g_WindowOffset + 179), Vector2f(1, 0), 0.5f);

	em.SpawnShootingEnemy(Point2f(g_WindowOffset + 369, g_WindowOffset + 123), 0.5f);
	em.SpawnShootingEnemy(Point2f(g_WindowOffset + 343, g_WindowOffset + 260), 0.4f);



	// Top
	em.SpawnTurretEnemy(Point2f(g_WindowOffset + 350, g_WindowOffset + 350), Vector2f(0, 1), 0.6f);
	em.SpawnTurretEnemy(Point2f(g_WindowOffset + 253, g_WindowOffset + 410), Vector2f(1, 0), 0.6f);



	// other things


	// Top Left
	em.SpawnWall(Rectf(g_WindowOffset, g_WindowOffset + 386, 212, 10));
	em.SpawnWall(Rectf(g_WindowOffset + 202, g_WindowOffset + 386, 10, 55));



	// Bottom Right
	em.SpawnWall(Rectf(g_WindowOffset + 588, g_WindowOffset + 70, 118, 10));
	em.SpawnWall(Rectf(g_WindowOffset + 588, g_WindowOffset + 70, 10, 370));

	em.SpawnEnemy(Point2f(g_WindowOffset + 640, g_WindowOffset + 90));

	em.SpawnWall(Rectf(g_WindowOffset + 654, g_WindowOffset + 130, 132, 10));
	em.SpawnWall(Rectf(g_WindowOffset + 588, g_WindowOffset + 242, 147, 10));

	em.SpawnWall(Rectf(g_WindowOffset + 690, g_WindowOffset + 337, 96, 10));

	em.SpawnMovingDeadlyWall(Rectf(g_WindowOffset + 622, g_WindowOffset + 337, 45, 10), 100.f, Vector2f(1, 0), 1.f, 44);



	// Bottom Left
	em.SpawnWall(Rectf(g_WindowOffset, g_WindowOffset + 70, 490, 10));
	em.SpawnDeadlyWall(Rectf(g_WindowOffset + 280, g_WindowOffset + 70, 180, 5), 50.f);

	em.SpawnSpeedPad(Rectf(g_WindowOffset + 100, g_WindowOffset, 90, 70), Vector2f(1, 0), 1000);
	em.SpawnSpeedPad(Rectf(g_WindowOffset + 190, g_WindowOffset, 90, 70), Vector2f(1, 0), 1000);

	em.SpawnSpeedPad(Rectf(g_WindowOffset + 280, g_WindowOffset, 90, 70), Vector2f(1, 0), 1000);
	em.SpawnSpeedPad(Rectf(g_WindowOffset + 370, g_WindowOffset, 90, 70), Vector2f(1, 0), 1000);

	em.SpawnSpeedPad(Rectf(g_WindowOffset + 280, g_WindowOffset, 90, 70), Vector2f(0, 1), 500);
	em.SpawnSpeedPad(Rectf(g_WindowOffset + 370, g_WindowOffset, 90, 70), Vector2f(0, 1), 500);

	em.SpawnSpeedPad(Rectf(g_WindowOffset + 500, g_WindowOffset, 90, 70), Vector2f(1, 0), 10000);

	em.SpawnCheckPoint(Rectf(g_WindowOffset + 500, g_WindowOffset + 80, 80, 80));


	em.SpawnWall(Rectf(g_WindowOffset + 480, g_WindowOffset + 70, 10, 90));
	em.SpawnWall(Rectf(g_WindowOffset + 414, g_WindowOffset + 150, 70, 10));

	em.SpawnWall(Rectf(g_WindowOffset + 480, g_WindowOffset + 242, 108, 10));

	em.SpawnWall(Rectf(g_WindowOffset + 410, g_WindowOffset + 205, 10, 70));

	em.SpawnWall(Rectf(g_WindowOffset + 240, g_WindowOffset + 308, 303, 10));
	em.SpawnWall(Rectf(g_WindowOffset + 240, g_WindowOffset + 70, 10, 240));

	em.SpawnWall(Rectf(g_WindowOffset + 303, g_WindowOffset + 250, 10, 67));

	em.SpawnWall(Rectf(g_WindowOffset + 534, g_WindowOffset + 309, 10, 61));

	em.SpawnMirrorArea(Rectf(g_WindowOffset + 255, g_WindowOffset + 262, 40, 40));


	// Left Middle
	em.SpawnMirrorArea(Rectf(g_WindowOffset + 205, g_WindowOffset + 90, 27, 27));
	em.SpawnMirrorArea(Rectf(g_Window.width - g_WindowOffset - 27 - 205, g_Window.height - g_WindowOffset - 27 - 90, 27, 27));


	em.SpawnWall(Rectf(g_WindowOffset + 114, g_WindowOffset + 134, 126, 60));
	em.SpawnWall(Rectf(g_WindowOffset + 114, g_WindowOffset + 134, 10, 252));
	em.SpawnWall(Rectf(g_WindowOffset, g_WindowOffset + 134, 50, 10));


	em.SpawnSpeedPad(Rectf(g_WindowOffset, g_WindowOffset + 80, 56, 54), Vector2f(-1, 0), 200);
	em.SpawnSpeedPad(Rectf(g_WindowOffset + 56, g_WindowOffset + 80, 58, 54), Vector2f(-1, 0), 200);

	em.SpawnDeadlyWall(Rectf(g_WindowOffset, g_WindowOffset + 80, 4, 54), 1000);

	em.SpawnDeadlyWall(Rectf(g_WindowOffset, g_WindowOffset + 152, 15, 70), 50);
	em.SpawnSpeedPad(Rectf(g_WindowOffset + 15, g_WindowOffset + 152, 99, 70), Vector2f(-1, 0), 500);

	em.SpawnCheckPoint(Rectf(g_WindowOffset + 135, g_WindowOffset + 88, 37, 37));

	em.SpawnWall(Rectf(g_WindowOffset, g_WindowOffset + 294, 54, 10));

	//em.SpawnSpeedPad(Rectf(g_WindowOffset + 54, g_WindowOffset + 252, 60, 64), Vector2f(0, 1), 200);
	em.SpawnSpeedPad(Rectf(g_WindowOffset + 54, g_WindowOffset + 286, 60, 90), Vector2f(0, 1), 300);

	em.SpawnDeadlyWall(Rectf(g_WindowOffset + 54, g_WindowOffset + 380, 60, 6), 1000);


	// Middle Top
	em.SpawnCheckPoint(Rectf(g_WindowOffset + 543, g_WindowOffset + 380, 37, 37));

	em.SpawnWall(Rectf(g_WindowOffset + 300, g_WindowOffset + 318, 10, 80));
	em.SpawnWall(Rectf(g_WindowOffset + 390, g_WindowOffset + 318, 10, 50));

	em.SpawnWall(Rectf(g_WindowOffset + 480, g_WindowOffset + 318, 10, 30));
	em.SpawnWall(Rectf(g_WindowOffset + 480, g_WindowOffset + 390, 10, 50));


	em.SpawnWinDoor(Rectf(g_WindowOffset + 146, g_WindowOffset + 205, 76, 76), false);


	// Discover areas
	UnlockWall* pUnlockWall{};
	pUnlockWall = em.SpawnUnlockWall(Rectf(g_WindowOffset + 480, g_WindowOffset + 252, 10, 56));
	em.SpawnUnlockArea(Rectf(g_WindowOffset + 425, g_WindowOffset + 85, 55, 55), pUnlockWall);

	pUnlockWall = em.SpawnUnlockWall(Rectf(g_WindowOffset + 544, g_WindowOffset + 360, 44, 10));
	em.SpawnUnlockArea(Rectf(g_WindowOffset + 10, g_WindowOffset + 330, 40, 40), pUnlockWall);


	// WinDoor
	em.SpawnWinDoor(Rectf(g_WindowOffset + 146, g_WindowOffset + 205, 76, 76), false);


}



