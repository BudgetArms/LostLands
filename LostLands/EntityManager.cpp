#include "pch.h"
#include "EntityManager.h"

#include <execution>
#include <ranges>

#include "Bullet.h"
#include "SpeedPad.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "Wall.h"




EntityManager::EntityManager() :
    m_Bullets{},
    m_SpeedPads{},
    m_Entities{},
    m_Player{}
{
}

EntityManager::~EntityManager()
{
}


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


    // killed all enemies
    if (m_Entities.empty())
    {
        m_bLevelComplete = true;
    }


    HandleBulletCollisions();
    HandleSpeedPadCollisions();
    HandleCharacterCollisions();

    for (auto& uWall : m_Walls)
        uWall->HandleCollisions(*m_Player);

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


void EntityManager::SpawnBullet(Character& owner, const Point2f& position, float angleDirection, float speed)
{
    m_Bullets.emplace_back(std::make_unique<Bullet>(owner, position, angleDirection, speed));
}

void EntityManager::SpawnPlayer(const Point2f& position)
{
    m_Player.reset();
    m_Player = std::make_unique<Player>(position);
}

void EntityManager::SpawnEnemy(const Point2f& position)
{
    m_Entities.emplace_back(std::make_unique<Enemy>(position));
}

void EntityManager::SpawnSpeedPad(const Point2f& position, const Vector2f& direction, float speed)
{
    m_SpeedPads.push_back(std::make_unique<SpeedPad>(Rectf(position.x, position.y, 90, 90), direction, speed));
}

void EntityManager::SpawnShootingEnemy(const Point2f& position)
{
    m_Entities.emplace_back(std::make_unique<Enemy>(position));
}

void EntityManager::SpawnEnemySpawner(const Point2f& position)
{
    //m_Entities.emplace_back(std::make_unique<EnemySpawner>(position, 0.3f));
}


void EntityManager::SpawnWall(const Rectf& area)
{
    m_Walls.emplace_back(std::make_unique<Wall>(area));
}



void EntityManager::Reset()
{
    m_Player.reset();
    m_Entities.clear();
    m_Bullets.clear();
    m_SpeedPads.clear();


    m_bLevelComplete = false;
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
        Rectf& bulletHitBox{ uBullet->GetHitBox() };
        // if Player Shot the bullet, check if bullet hits enemy
        if (dynamic_cast<Player*>(uBullet->GetOwner()))
        {
            for (auto& uEntity : m_Entities)
            {
                if (utils::IsOverlapping(uEntity->GetHitBox(), bulletHitBox))
                {
                    uEntity->Damage(uBullet->GetDamage());
                    uBullet->Destroy();
                }
            }
        }
        else if (dynamic_cast<Enemy*>(uBullet->GetOwner()))
        {
            if (utils::IsOverlapping(m_Player->GetHitBox(), bulletHitBox))
            {
                m_Player->Damage(uBullet->GetDamage());
                uBullet->Destroy();
            }
        }
    }

}

void EntityManager::HandleSpeedPadCollisions()
{
    if (!m_Player)
        return;

    const Rectf& playerHitBox{ m_Player->GetHitBox() };

    for (auto& uSpeedPads : m_SpeedPads)
        if (utils::IsOverlapping(uSpeedPads->GetHitBox(), playerHitBox))
            uSpeedPads->OnCollision(m_Player.get());

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
            const Vector2f directionAwayFromEnemy{ (m_Player->GetPosition() - uEntity->GetPosition()).Normalized() };
            m_Player->Damage(10.f);
            uEntity->Damage(10.f);
            m_Player->SetVelocity(100 * directionAwayFromEnemy);
        }
    }

}
