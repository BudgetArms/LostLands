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

void DeadlyWall::HandleCollisions(Player& player)
{
    const Rectf& playerHitBox{ player.GetHitBox() };
    const Rectf area{ GetArea() };
    if (!utils::IsOverlapping(playerHitBox, area))
        return;

    Point2f playerPos{ player.m_Position };
    Vector2f playerVelocity = player.m_Velocity;

    // hit left wall
    if (playerPos.x - playerHitBox.width / 2 <= area.left)
    {
        playerPos.x = area.left + playerHitBox.width / 2;
        player.Damage(m_Damage);

        if (player.IsDashing())
            playerVelocity.x *= -1;
        else
            playerVelocity.x *= -player.GetBouncinessWalls();
    }

    // hit right wall
    if (playerPos.x + playerHitBox.width / 2 >= area.left + area.width)
    {
        playerPos.x = area.left + area.width - playerHitBox.width / 2;
        player.Damage(m_Damage);

        if (player.IsDashing())
            playerVelocity.x *= -1;
        else
            playerVelocity.x *= -player.GetBouncinessWalls();
    }

    // hit bottom wall
    if (playerPos.y - playerHitBox.height / 2 <= area.bottom)
    {
        playerPos.y = area.bottom + playerHitBox.height / 2;
        player.Damage(m_Damage);

        if (player.IsDashing())
            playerVelocity.y *= -1;
        else
            playerVelocity.y *= -player.GetBouncinessWalls();
    }

    // hit top wall
    if (playerPos.y + playerHitBox.height / 2 >= area.bottom + area.height)
    {
        playerPos.y = area.bottom + area.height - playerHitBox.height / 2;
        player.Damage(m_Damage);

        if (player.IsDashing())
            playerVelocity.y *= -1;
        else
            playerVelocity.y *= -player.GetBouncinessWalls();
    }


    player.m_Velocity = playerVelocity;

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


