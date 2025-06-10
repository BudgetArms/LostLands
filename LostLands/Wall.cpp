#include "pch.h"
#include "Wall.h"

#include "Character.h"
#include "Player.h"
#include "Bullet.h"


Wall::Wall(const Rectf& area) :
    m_Area{ area }
{
}

void Wall::Draw() const
{
    utils::SetColor(1, 1, 1, 1.f);
    utils::FillRect(m_Area);
}


void Wall::HandleCollisions(Player& player)
{
    if (player.IsDead())
        return;

    const Rectf& playerHitBox{ player.GetHitBox() };

    // aabb
    if (!utils::IsOverlapping(playerHitBox, m_Area))
        return;

    if (!utils::IsOverlapping(m_Area, Circlef(player.m_Position, playerHitBox.width / 2)))
        return;


    Point2f playerPos{ player.m_Position };
    Vector2f playerVelocity = player.m_Velocity;


    if (playerPos.x < m_Area.left)
    {
        // hit left wall
        playerPos.x = m_Area.left - playerHitBox.width / 2;
        if (player.IsDashing())
            playerVelocity.x *= -1;
        else
            playerVelocity.x *= -player.GetBouncinessWalls();

    }
    else if (playerPos.x > m_Area.left + m_Area.width)
    {
        // hit right wall
        playerPos.x = m_Area.left + m_Area.width + playerHitBox.width / 2;
        if (player.IsDashing())
            playerVelocity.x *= -1;
        else
            playerVelocity.x *= -player.GetBouncinessWalls();

    }
    else if (playerPos.y > m_Area.bottom + m_Area.height)
    {
        // hit top wall
        playerPos.y = m_Area.bottom + m_Area.height + playerHitBox.height / 2;
        if (player.IsDashing())
            playerVelocity.y *= -1;
        else
            playerVelocity.y *= -player.GetBouncinessWalls();
    }
    else if (playerPos.y < m_Area.bottom)
    {
        // hit bottom wall
        playerPos.y = m_Area.bottom - playerHitBox.height / 2;
        if (player.IsDashing())
            playerVelocity.y *= -1;
        else
            playerVelocity.y *= -player.GetBouncinessWalls();
    }

    player.m_Position = playerPos;
    player.m_Velocity = playerVelocity;
}

void Wall::HandleCollisionsBullet(Bullet& bullet)
{
    const Rectf& bulletHitBox{ bullet.GetHitBox() };

    // aabb
    if (!utils::IsOverlapping(m_Area, bulletHitBox))
        return;

    if (!utils::IsOverlapping(m_Area, Circlef(bullet.m_Position, bulletHitBox.width / 2)))
        return;


    Point2f bulletPos{ bullet.m_Position };
    Vector2f bulletDirection = bullet.m_Direction;

    if (bulletPos.x < m_Area.left)
    {
        // hit left wall
        bulletPos.x = m_Area.left + bulletHitBox.width / 2;
        bulletDirection.x *= -1;
    }
    else if (bulletPos.x > m_Area.left + m_Area.width)
    {
        // hit right wall
        bulletPos.x = m_Area.left + m_Area.width - bulletHitBox.width / 2;
        bulletDirection.x *= -1;
    }
    else if (bulletPos.y < m_Area.bottom)
    {
        // hit bottom wall
        bulletPos.y = m_Area.bottom + bulletHitBox.height / 2;
        bulletDirection.y *= -1;
    }
    else if (bulletPos.y > m_Area.bottom + m_Area.height)
    {
        // hit top wall
        bulletPos.y = m_Area.bottom + m_Area.height - bulletHitBox.height / 2;
        bulletDirection.y *= -1;
    }

    bullet.m_Direction = bulletDirection;
}


