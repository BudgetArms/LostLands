#include "pch.h"
#include "DeadlyWall.h"

#include "Player.h"
#include "Bullet.h"


DeadlyWall::DeadlyWall(const Rectf& area, float damage) :
	Wall(area),
	m_Damage{ damage }
{
}

void DeadlyWall::Draw() const
{
	utils::SetColor(1, 0, 0, 1.f);
	utils::FillRect(GetArea());
}

void DeadlyWall::HandleCollisionsPlayer(Player& player)
{
	if (player.IsDead())
		return;

	const Rectf& playerHitbox{ player.GetHitBox() };

	if (!utils::IsOverlapping(playerHitbox, m_Area))
		return;

	if (!utils::IsOverlapping(m_Area, Circlef(player.m_Position, playerHitbox.width / 2)))
		return;


	if (player.IsDashing())
		HandleCollisions(playerHitbox, player.m_Position, player.m_Velocity, player.GetBouncinessWalls());
	else
		HandleCollisions(playerHitbox, player.m_Position, player.m_Velocity, 1.f);

	player.Damage(m_Damage);

}


void DeadlyWall::HandleCollisionsBullet(Bullet& bullet)
{
	const Rectf& bulletHitBox{ bullet.GetHitBox() };
	const Rectf area{ GetArea() };

	// aabb
	if (!utils::IsOverlapping(area, bulletHitBox))
		return;

	if (!utils::IsOverlapping(area, Circlef(bullet.m_Position, bulletHitBox.width / 2)))
		return;

	bullet.Destroy();
}



