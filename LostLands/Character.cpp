#include "pch.h"
#include "Character.h"

#include "EntityManager.h"
#include "Bullet.h"


Character::Character(const Point2f& position) :
    m_Position{ position }
{
}


void Character::SetHealth(float health)
{
    if (m_Health < 0 || m_bIsDead)
        return;

    m_Health = health;

    if (m_Health > m_MaxHealth)
        m_Health = m_MaxHealth;

}


void Character::Shoot()
{

    if (m_AccumulatedSecBullets < (1.f / m_BulletsPerSecond))
        return;

    m_AccumulatedSecBullets = 0;

    Bullet bullet{ *this, m_Position, 0.f };
    //Bullet bullet{ *this, m_Position, m_DirectionAngle };
    float directionAngle = 90.f;

    EntityManager::GetInstance().SpawnBullet(*this, m_Position, directionAngle);
}

void Character::Update(float elapsedSec)
{
    if (m_bIsDead)
    {
        m_bIsMarkedForDeletion = true;

    }
}

void Character::Damage(float health)
{
    m_Health -= health;

    if (m_Health <= 0)
    {
        m_bIsDead = true;
        m_DestroyDelayCooldown = m_DestroyDelayTime;
        return;
    }

    if (m_Health > m_MaxHealth)
        m_Health = m_MaxHealth;


}
