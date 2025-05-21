#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

#include "Singleton.h"
#include "Player.h"

class Bullet;
class SpeedPad;
class Character;
//class Player;


class EntityManager final : public Singleton<EntityManager>
{
public:
    void Draw() const;
    void Update(float elapsedSec);
    void LateUpdate();


    void SpawnBullet(Character& owner, const Point2f& position, float angleDirection, float speed = 400.f);

    void SpawnPlayer(const Point2f& position);
    void SpawnEnemy(const Point2f& position);
    void SpawnSpeedPad(const Point2f& position, const Vector2f& direction, float speed);
    void SpawnShootingEnemy(const Point2f& position);
    void SpawnEnemySpawner(const Point2f& position);

    std::vector<std::unique_ptr<Bullet>>& GetBullets() { return m_Bullets; };
    std::vector<std::unique_ptr<Character>>& GetEnemies() { return m_Entities; };

    Player* GetPlayer() const { return m_Player.get(); };

    void Reset();
    bool IsLevelFinished() const;

    void HandleBulletCollisions();
    void HandleSpeedPadCollisions();
    void HandleCharacterCollisions();


private:
    friend class Singleton<EntityManager>;
    EntityManager();
    ~EntityManager();


    std::vector<std::unique_ptr<Bullet>> m_Bullets;
    std::vector<std::unique_ptr<SpeedPad>> m_SpeedPads;
    std::vector<std::unique_ptr<Character>> m_Entities;
    std::unique_ptr<Player> m_Player;

    bool m_bLevelComplete{ false };


};

