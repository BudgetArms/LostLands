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
	if (m_bIsDead)
		return;

	m_Health = health;

	if (m_Health > m_MaxHealth)
		m_Health = m_MaxHealth;

}


void Character::Update(float elapsedSec)
{
	if (m_bIsDead)
		m_bIsMarkedForDeletion = true;

	m_AccumulatedSecBullets += elapsedSec;
	if (m_AccumulatedSecBullets >= 1 / m_BulletsPerSecond)
	{
		m_AccumulatedSecBullets = 0.f;
		m_bCanShoot = true;
	}
}

void Character::Damage(float health)
{
	m_Health -= health;

	if (m_Health <= 0)
	{
		m_bIsDead = true;
		return;
	}

	if (m_Health > m_MaxHealth)
		m_Health = m_MaxHealth;

}


