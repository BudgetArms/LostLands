#include "pch.h"
#include "Enemy.h"

#include <iostream>

#include "EntityManager.h"
#include "Player.h"


Enemy::Enemy(const Point2f& position) :
	Character(position)
{
	m_Color = Color4f(1.f, 0.f, 0.f, 1.f);
	m_HealthBarColor = Color4f(0.f, 0.f, 1.f, 0.8f);
	m_Width = 20;
	m_Height = 20;
	m_Speed = 100.f;
	m_MaxHealth = 100.f;
	SetHealth(m_MaxHealth);

	m_HitBox.left = m_Position.x;
	m_HitBox.bottom = m_Position.y;
	m_HitBox.width = m_Width;
	m_HitBox.height = m_Height;

	m_BulletsPerSecond = 1.f;


}


void Enemy::Draw() const
{
	auto hitBox = Rectf(m_Position.x - m_Width / 2, m_Position.y - m_Height / 2, m_Width, m_Height);

	// Draw Enemy
	utils::SetColor(m_Color);
	utils::FillRect(hitBox);

	// Draw HealthBar
	constexpr float healthBarWidthOffset{ 25.f };
	constexpr float healthBarHeightOffset{ 20.f };
	utils::SetColor(m_HealthBarColor);
	utils::FillRect(m_Position.x - healthBarWidthOffset, m_Position.y + healthBarHeightOffset,
		(GetHealth() / m_MaxHealth) * (2 * healthBarWidthOffset), 10.f);

	utils::SetColor(1.f, 1.f, 1.f, 1.f);
	utils::DrawRect(m_Position.x - healthBarWidthOffset, m_Position.y + healthBarHeightOffset,
		(2 * healthBarWidthOffset), 10.f, 2.f);

	if (g_bShowHitboxes)
	{
		Rectf hitBox{ m_Position.x - m_Width / 2, m_Position.y - m_Height / 2, m_Width, m_Height };
		utils::SetColor(g_HitBoxesColor);
		utils::DrawRect(hitBox);
	}

}

void Enemy::Update(float elapsedSec)
{
	if (!m_bIsEnabled)
		return;

	Character::Update(elapsedSec);
	Shoot();
}


Rectf& Enemy::GetHitBox()
{
	// TODO: insert return statement here
	m_HitBox.left = m_Position.x - m_Width / 2;
	m_HitBox.bottom = m_Position.y - m_Height / 2;
	m_HitBox.width = m_Width;
	m_HitBox.height = m_Height;
	return m_HitBox;
}

void Enemy::Shoot()
{
	const Player* player = EntityManager::GetInstance().GetPlayer();
	if (!player)
	{
		std::cout << "Enemy::Shoot: Player not found\n";
		return;
	}

	if (player->IsDead())
		return;

	if (!m_bCanShoot || !m_bIsShootingEnabled)
		return;

	const Point2f directionToPlayer{ (player->m_Position - m_Position).Normalized() };
	const float angleToPlayer{ atan2f(directionToPlayer.y, directionToPlayer.x) };

	EntityManager::GetInstance().SpawnBullet(BulletType::Enemy, m_Position, utils::g_toDegrees * angleToPlayer);
	m_bCanShoot = false;

}


