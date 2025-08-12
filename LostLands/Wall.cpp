#include "pch.h"
#include "Wall.h"

#include "Character.h"
#include "Player.h"
#include "Bullet.h"


Wall::Wall(const Rectf& area) :
	m_Area{ area }
{
}

Wall::~Wall() {};

void Wall::Draw() const
{
	if (!m_bRenderWall)
		return;

	utils::SetColor(1, 1, 1, 1.f);
	utils::FillRect(m_Area);
}


void Wall::HandleCollisionsPlayer(Player& player)
{
	if (player.IsDead())
		return;

	const Rectf& playerHitbox{ player.GetHitBox() };

	// aabb
	if (!utils::IsOverlapping(playerHitbox, m_Area))
		return;

	if (!utils::IsOverlapping(m_Area, Circlef(player.m_Position, playerHitbox.width / 2)))
		return;


	if (player.IsDashing())
		HandleCollisions(playerHitbox, player.m_Position, player.m_Velocity, player.GetBouncinessWalls());
	else
		HandleCollisions(playerHitbox, player.m_Position, player.m_Velocity, 1.f);

}

void Wall::HandleCollisionsBullet(Bullet& bullet)
{
	const Rectf& bulletHitBox{ bullet.GetHitBox() };

	// aabb
	if (!utils::IsOverlapping(m_Area, bulletHitBox))
		return;

	if (!utils::IsOverlapping(m_Area, Circlef(bullet.m_Position, bulletHitBox.width / 2)))
		return;


	HandleCollisions(bulletHitBox, bullet.m_Position, bullet.m_Direction, 1.f);

}

void Wall::HandleCollisions(const Rectf& hitbox, Point2f& outPos, Vector2f& outVelocity, float bouncinessWalls)
{
	if (!utils::IsOverlapping(hitbox, m_Area))
		return;

	if (outPos.x < m_Area.left)
	{
		// hit left wall
		outPos.x = m_Area.left - hitbox.width / 2.f - m_OffsetAfterCollisions;

		if (outVelocity.x > 0)
			outVelocity.x *= -bouncinessWalls;
	}
	else if (outPos.x > m_Area.left + m_Area.width)
	{
		// hit right wall
		outPos.x = m_Area.left + m_Area.width + hitbox.width / 2.f + m_OffsetAfterCollisions;

		if (outVelocity.x < 0)
			outVelocity.x *= -bouncinessWalls;
	}
	else if (outPos.y < m_Area.bottom)
	{
		// hit bottom wall
		outPos.y = m_Area.bottom - hitbox.height / 2.f - m_OffsetAfterCollisions;

		if (outVelocity.y > 0)
			outVelocity.y *= -bouncinessWalls;
	}
	else if (outPos.y > m_Area.bottom + m_Area.height)
	{
		// hit top wall
		outPos.y = m_Area.bottom + m_Area.height + hitbox.height / 2.f + m_OffsetAfterCollisions;

		if (outVelocity.y < 0)
			outVelocity.y *= -bouncinessWalls;
	}

}


