#include "pch.h"
#include "EntityManager.h"

#include <execution>
#include <ranges>
#include <algorithm>

#include "LevelManager.h"

#include "Bullet.h"
#include "SpeedPad.h"
#include "Character.h"
#include "Enemy.h"
#include "TurretEnemy.h"
#include "Player.h"
#include "Wall.h"
#include "DeadlyWall.h"
#include "CheckPoint.h"
#include "MovingDeadlyWall.h"
#include "WinDoor.h"
#include "MirrorArea.h"
#include "UnlockArea.h"
#include "UnlockWall.h"



EntityManager::EntityManager() :
	m_Bullets{},
	m_SpeedPads{},
	m_Entities{},
	m_Walls{},
	m_Player{},
	m_WinDoors{}
{
}

EntityManager::~EntityManager() {};


void EntityManager::Draw() const
{
	for (auto& uEntity : m_Entities)
		uEntity->Draw();

	for (auto& uBullet : m_Bullets)
		uBullet->Draw();

	for (auto& uSpeedPad : m_SpeedPads)
		uSpeedPad->Draw();

	for (auto& uWall : m_Walls)
		uWall->Draw();

	for (auto& uCheckPoint : m_CheckPoints)
		uCheckPoint->Draw();

	for (auto& uWinDoor : m_WinDoors)
		uWinDoor->Draw();

	for (auto& mirrorArea : m_MirrorAreas)
		mirrorArea.Draw();

	for (auto& uUnlockArea : m_UnlockAreas)
		uUnlockArea->Draw();


	if (m_Player)
		m_Player->Draw();

}

void EntityManager::Update(float elapsedSec)
{

	if (m_Player)
		m_Player->Update(elapsedSec);


	for (auto& uEntity : m_Entities)
		uEntity->Update(elapsedSec);

	for (auto& uBullet : m_Bullets)
		uBullet->Update(elapsedSec);

	for (auto& uWall : m_Walls)
		if (auto* movingDeadlyWall = dynamic_cast<MovingDeadlyWall*>(uWall.get()))
			movingDeadlyWall->Update(elapsedSec);

	for (auto& uWinDoor : m_WinDoors)
		uWinDoor->Update();


	// killed all enemies
	if (m_Entities.empty())
		m_bLevelComplete = true;


	HandleBulletCollisions();
	HandleSpeedPadCollisions(elapsedSec);
	HandleCharacterCollisions();

	for (auto& uWall : m_Walls)
		uWall->HandleCollisionsPlayer(*m_Player);

	for (auto& uWall : m_Walls)
		for (auto& uBullet : m_Bullets)
			uWall->HandleCollisionsBullet(*uBullet);

	if (m_Player)
		for (auto& uCheckPoint : m_CheckPoints)
			if (utils::IsOverlapping(m_Player->GetHitBox(), uCheckPoint->GetArea()))
				LevelManager::GetInstance().m_CurrentCheckPoint = uCheckPoint.get();

	if (m_Player)
		for (auto& uUnlockArea : m_UnlockAreas)
			if (utils::IsOverlapping(m_Player->GetHitBox(), uUnlockArea->GetArea()))
			{
				auto pWall = uUnlockArea->GetWall();
				std::erase_if(m_Walls, [pWall](const auto& uWall)
					{
						return uWall.get() == pWall;
					});
			}



	LateUpdate();

}

void EntityManager::LateUpdate()
{
	std::erase_if(m_Entities,
		[](auto& entity)
		{
			return entity->IsMarkedForDeletion();
		});


	std::erase_if(m_Bullets,
		[](auto& bullet)
		{
			return bullet->IsMarkedForDeletion();
		});

}

void EntityManager::Reset()
{
	m_Bullets.clear();
	m_SpeedPads.clear();
	m_Entities.clear();
	m_Walls.clear();
	m_CheckPoints.clear();
	m_Player.reset();
	m_WinDoors.clear();
	m_MirrorAreas.clear();
	m_UnlockAreas.clear();

	m_bLevelComplete = false;
}



Bullet* EntityManager::SpawnBullet(BulletType characterType, const Point2f& position, float angleDirection, float speed)
{
	auto bullet = std::make_unique<Bullet>(characterType, position, angleDirection, speed);
	auto pBullet = bullet.get();

	m_Bullets.emplace_back(std::move(bullet));
	return pBullet;
}

Player* EntityManager::SpawnPlayer(const Point2f& position, int lives)
{
	m_Player.reset();
	m_Player = std::make_unique<Player>(position, lives);
	return m_Player.get();
}

Enemy* EntityManager::SpawnEnemy(const Point2f& position)
{
	auto enemy = std::make_unique<Enemy>(position);
	auto pEnemy = enemy.get();

	m_Entities.emplace_back(std::move(enemy));
	return pEnemy;
}

SpeedPad* EntityManager::SpawnSpeedPad(const Point2f& position, const Vector2f& direction, float speed)
{
	auto speedPad = std::make_unique<SpeedPad>(Rectf(position.x, position.y, 90, 90), direction, speed);
	auto pSpeedPad = speedPad.get();

	m_SpeedPads.emplace_back(std::move(speedPad));
	return pSpeedPad;
}

SpeedPad* EntityManager::SpawnSpeedPad(const Rectf& area, const Vector2f& direction, float speed)
{
	auto speedPad = std::make_unique<SpeedPad>(area, direction, speed);
	auto pSpeedPad = speedPad.get();

	m_SpeedPads.emplace_back(std::move(speedPad));
	return pSpeedPad;
}

Enemy* EntityManager::SpawnShootingEnemy(const Point2f& position, float bulletsPerSecond)
{
	auto enemy = std::make_unique<Enemy>(position);
	auto pEnemy = enemy.get();
	enemy->m_bIsShootingEnabled = true;
	enemy->m_BulletsPerSecond = bulletsPerSecond;
	enemy->m_Color = Color4f(1.f, 0.2f, 0.0f, 1.f);

	m_Entities.emplace_back(std::move(enemy));
	return pEnemy;

}

TurretEnemy* EntityManager::SpawnTurretEnemy(const Point2f& position, const Vector2f& direction, float bulletsPerSecond)
{
	auto turret = std::make_unique<TurretEnemy>(position, direction);
	auto pTurret = turret.get();
	turret->m_bIsShootingEnabled = true;
	turret->m_BulletsPerSecond = bulletsPerSecond;
	turret->m_Color = Color4f(1.f, 0.2f, 0.0f, 1.f);

	m_Entities.emplace_back(std::move(turret));
	return pTurret;
}

Wall* EntityManager::SpawnWall(const Rectf& area)
{
	auto wall = std::make_unique<Wall>(area);
	auto pWall = wall.get();

	m_Walls.emplace_back(std::move(wall));
	return pWall;
}

DeadlyWall* EntityManager::SpawnDeadlyWall(const Rectf& area, float damage)
{
	auto wall = std::make_unique<DeadlyWall>(area, damage);
	auto pWall = wall.get();

	m_Walls.emplace_back(std::move(wall));
	return pWall;

}

CheckPoint* EntityManager::SpawnCheckPoint(const Rectf& area)
{
	auto checkPoint = std::make_unique<CheckPoint>(area);
	auto pCheckPoint = checkPoint.get();

	m_CheckPoints.emplace_back(std::move(checkPoint));
	return pCheckPoint;
}

MovingDeadlyWall* EntityManager::SpawnMovingDeadlyWall(const Rectf& area, float damage, const Vector2f& direction, float speed, float range)
{
	auto movingDeadlyWall = std::make_unique<MovingDeadlyWall>(area, damage, direction, speed, range);
	auto pMovingDeadlyWall = movingDeadlyWall.get();

	m_Walls.emplace_back(std::move(movingDeadlyWall));
	return pMovingDeadlyWall;

}

WinDoor* EntityManager::SpawnWinDoor(const Rectf& area, bool needsAllEnemiesKilled)
{
	auto winDoor = std::make_unique<WinDoor>(area, needsAllEnemiesKilled);
	auto pWinDoor = winDoor.get();

	m_WinDoors.emplace_back(std::move(winDoor));
	return pWinDoor;
}

void EntityManager::SpawnMirrorArea(const Rectf& area)
{
	m_MirrorAreas.emplace_back(area);
}


UnlockWall* EntityManager::SpawnUnlockWall(const Rectf& area)
{
	auto unlockWall = std::make_unique<UnlockWall>(area);
	auto pUnlockWall = unlockWall.get();

	m_Walls.emplace_back(std::move(unlockWall));
	return pUnlockWall;
}


void EntityManager::SpawnUnlockArea(const Rectf& area, UnlockWall* pUnlockWall)
{
	auto unlockArea = std::make_unique<UnlockArea>(area, pUnlockWall);
	auto pUnlockArea = unlockArea.get();

	m_UnlockAreas.emplace_back(std::move(unlockArea));
	//return pUnlockArea;
}





bool EntityManager::IsLevelFinished() const
{
	return m_bLevelComplete;
}

void EntityManager::HandleBulletCollisions()
{
	if (!m_Player)
		return;

	for (auto& uBullet : m_Bullets)
	{
		if (uBullet->IsMarkedForDeletion())
			return;

		Rectf& bulletHitBox{ uBullet->GetHitBox() };

		// if Player Shot the bullet, check if bullet hits enemy
		if (uBullet->GetType() == BulletType::Player)
		{
			for (auto& uEntity : m_Entities)
			{
				if (utils::IsOverlapping(uEntity->GetHitBox(), bulletHitBox))
				{
					uEntity->Damage(uBullet->m_Damage);
					uBullet->Destroy();
				}
			}
		}
		else if (uBullet->GetType() == BulletType::Enemy)
		{
			if (utils::IsOverlapping(m_Player->GetHitBox(), bulletHitBox))
			{
				m_Player->Damage(uBullet->m_Damage);
				uBullet->Destroy();
			}
		}
	}

}

void EntityManager::HandleSpeedPadCollisions(float elapsedSec)
{
	if (!m_Player)
		return;

	const Rectf& playerHitBox{ m_Player->GetHitBox() };

	for (auto& uSpeedPads : m_SpeedPads)
		if (utils::IsOverlapping(uSpeedPads->GetHitBox(), playerHitBox))
			uSpeedPads->OnCollision(m_Player.get(), elapsedSec);

}

void EntityManager::HandleCharacterCollisions()
{
	if (!m_Player)
		return;

	const Rectf& playerHitBox{ m_Player->GetHitBox() };

	for (auto& uEntity : m_Entities)
	{
		if (utils::IsOverlapping(uEntity->GetHitBox(), playerHitBox))
		{
			const Vector2f directionAwayFromEnemy{ (m_Player->m_Position - uEntity->m_Position).Normalized() };
			m_Player->Damage(10.f);
			uEntity->Damage(10.f);
			m_Player->m_Velocity = 100 * directionAwayFromEnemy;
		}
	}

}


