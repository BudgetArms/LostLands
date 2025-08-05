#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

#include "Singleton.h"

enum class BulletType
{
	Player,
	Enemy
};

class Bullet;
class SpeedPad;
class Character;
class Enemy;
class TurretEnemy;
class Player;
class Wall;
class DeadlyWall;
class CheckPoint;
class MovingDeadlyWall;
class WinDoor;
class MirrorArea;
class UnlockArea;
class UnlockWall;

class EntityManager final : public Singleton<EntityManager>
{
public:
	void Draw() const;
	void Update(float elapsedSec);
	void LateUpdate();

	void Reset();

	Player* SpawnPlayer(const Point2f& position, int lives);
	Enemy* SpawnEnemy(const Point2f& position);
	SpeedPad* SpawnSpeedPad(const Point2f& position, const Vector2f& direction, float speed);
	SpeedPad* SpawnSpeedPad(const Rectf& rect, const Vector2f& direction, float speed);
	Bullet* SpawnBullet(BulletType characterType, const Point2f& position, float angleDirection, float speed = 400.f);
	Enemy* SpawnShootingEnemy(const Point2f& position, float bulletsPerSecond = 1.f);
	TurretEnemy* SpawnTurretEnemy(const Point2f& position, const Vector2f& direction, float bulletsPerSecond = 1.f);
	Wall* SpawnWall(const Rectf& area);
	DeadlyWall* SpawnDeadlyWall(const Rectf& area, float damage);
	CheckPoint* SpawnCheckPoint(const Rectf& area);
	MovingDeadlyWall* SpawnMovingDeadlyWall(const Rectf& area, float damage, const Vector2f& direction, float speed, float range);
	WinDoor* SpawnWinDoor(const Rectf& area, bool needsAllEnemiesKilled);
	void SpawnMirrorArea(const Rectf& area);
	UnlockWall* SpawnUnlockWall(const Rectf& area);
	void SpawnUnlockArea(const Rectf& area, UnlockWall* pUnlockWall);

	std::vector<std::unique_ptr<Bullet>>& GetBullets() { return m_Bullets; };
	std::vector<std::unique_ptr<Character>>& GetEnemies() { return m_Entities; };
	std::vector<std::unique_ptr<WinDoor>>& GetWinDoors() { return m_WinDoors; };
	const std::vector<MirrorArea>& GetMirrorAreas() const { return m_MirrorAreas; };

	Player* GetPlayer() const { return m_Player.get(); };

	bool IsLevelFinished() const;

	void HandleBulletCollisions();
	void HandleSpeedPadCollisions(float elapsedSec);
	void HandleCharacterCollisions();


private:
	friend class Singleton<EntityManager>;
	EntityManager();
	~EntityManager(); // empty bc class forwarding

	// the code quality does not matters, and time limits, so whom cares
	friend class LevelManager;


	std::vector<std::unique_ptr<Bullet>> m_Bullets;
	std::vector<std::unique_ptr<SpeedPad>> m_SpeedPads;
	std::vector<std::unique_ptr<Character>> m_Entities;
	std::vector<std::unique_ptr<Wall>> m_Walls;
	std::vector<std::unique_ptr<CheckPoint>> m_CheckPoints;
	std::unique_ptr<Player> m_Player;
	std::vector<std::unique_ptr<WinDoor>> m_WinDoors;
	std::vector<MirrorArea> m_MirrorAreas;

	std::vector<std::unique_ptr<UnlockArea>> m_UnlockAreas;
	//std::vector<std::unique_ptr<UnlockWall>> m_UnlockWalls;


	bool m_bLevelComplete{ false };


};


