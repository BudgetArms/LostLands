#include "pch.h"
#include "DeadlyWall.h"

#include "Player.h"


DeadlyWall::DeadlyWall(const Rectf& area, float damage) :
    Wall(area),
    m_Damage{ damage }
{
}

void DeadlyWall::Draw() const
{
    utils::SetColor(1, 0, 0, 0.9f);
    utils::FillRect(m_Area);
}

void DeadlyWall::HandleCollisions(Player& player)
{
    const Rectf& playerHitBox{ player.GetHitBox() };
    if (!utils::IsOverlapping(playerHitBox, m_Area))
        return;

    Point2f playerPos{ player.GetPosition() };

    Vector2f playerVelocity = player.GetVelocity();

    // hit left wall
    if (playerPos.x - playerHitBox.width / 2 <= m_Area.left)
    {
        playerPos.x = m_Area.left + playerHitBox.width / 2;
        player.Damage(m_Damage);
        if (player.IsDashing())
            playerVelocity.x *= -1;
        else
            playerVelocity.x *= -player.GetBouncinessWalls();
    }

    // hit right wall
    if (playerPos.x + playerHitBox.width / 2 >= m_Area.left + m_Area.width)
    {
        playerPos.x = m_Area.left + m_Area.width - playerHitBox.width / 2;
        player.Damage(m_Damage);

        if (player.IsDashing())
            playerVelocity.x *= -1;
        else
            playerVelocity.x *= -player.GetBouncinessWalls();
    }

    // hit bottom wall
    if (playerPos.y - playerHitBox.height / 2 <= m_Area.bottom)
    {
        playerPos.y = m_Area.bottom + playerHitBox.height / 2;
        player.Damage(m_Damage);

        if (player.IsDashing())
            playerVelocity.y *= -1;
        else
            playerVelocity.y *= -player.GetBouncinessWalls();
    }

    // hit top wall
    if (playerPos.y + playerHitBox.height / 2 >= m_Area.bottom + m_Area.height)
    {
        playerPos.y = m_Area.bottom + m_Area.height - playerHitBox.height / 2;
        player.Damage(m_Damage);

        if (player.IsDashing())
            playerVelocity.y *= -1;
        else
            playerVelocity.y *= -player.GetBouncinessWalls();
    }


    player.SetVelocity(playerVelocity);

}


