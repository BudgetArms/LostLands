#include "pch.h"
#include "TurretEnemy.h"

#include "EntityManager.h"


TurretEnemy::TurretEnemy(const Point2f& position, const Vector2f& direction) :
	Enemy(position),
	m_Direction{ direction.Normalized() }
{
}


void TurretEnemy::Draw() const
{
	auto hitBox = Rectf(m_Position.x - m_Width / 2, m_Position.y - m_Height / 2, m_Width, m_Height);

	// Draw TurretEnemy
	utils::SetColor(m_Color);
	utils::FillTriangle(Point2f(m_Position.x - m_Width / 2.f, m_Position.y - m_Height / 2.f), Point2f(m_Position.x, m_Position.y + m_Height / 2.f),
		Point2f(m_Position.x + m_Width / 2.f, m_Position.y - m_Height / 2.f));

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

void TurretEnemy::Shoot()
{

	if (!m_bCanShoot || !m_bIsShootingEnabled)
		return;

	const float angleTurret{ atan2f(m_Direction.y, m_Direction.x) };

	EntityManager::GetInstance().SpawnBullet(BulletType::Enemy, m_Position, utils::g_toDegrees * angleTurret);
	m_bCanShoot = false;

}
