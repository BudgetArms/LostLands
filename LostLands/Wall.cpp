#include "pch.h"
#include "Wall.h"

#include "Character.h"
#include "Player.h"


Wall::Wall(const Rectf& area) :
    m_HitBox{ area },
    m_bIsActive{ true }
{
}

void Wall::Draw() const
{
    utils::SetColor(1, 1, 1, 1);
    utils::FillRect(m_HitBox);
}


void Wall::HandleCollisions(Player& player)
{
    const Rectf& playerHitBox{ player.GetHitBox() };
    if (!utils::IsOverlapping(playerHitBox, m_HitBox))
        return;

    Point2f playerPos{ player.GetPosition() };

    Vector2f playerVelocity = player.GetVelocity();

    // hit left wall
    if (playerPos.x - playerHitBox.width / 2 <= m_HitBox.left)
    {
        playerPos.x = m_HitBox.left + playerHitBox.width / 2;
        if (player.IsDashing())
            playerVelocity.x *= -1;
        else
            playerVelocity.x *= -player.GetBouncinessWalls();
    }

    // hit right wall
    if (playerPos.x + playerHitBox.width / 2 >= m_HitBox.left + m_HitBox.width)
    {
        playerPos.x = m_HitBox.left + m_HitBox.width - playerHitBox.width / 2;
        if (player.IsDashing())
            playerVelocity.x *= -1;
        else
            playerVelocity.x *= -player.GetBouncinessWalls();
    }

    // hit bottom wall
    if (playerPos.y - playerHitBox.height / 2 <= m_HitBox.bottom)
    {
        playerPos.y = m_HitBox.bottom + playerHitBox.height / 2;
        if (player.IsDashing())
            playerVelocity.y *= -1;
        else
            playerVelocity.y *= -player.GetBouncinessWalls();
    }

    // hit top wall
    if (playerPos.y + playerHitBox.height / 2 >= m_HitBox.bottom + m_HitBox.height)
    {
        playerPos.y = m_HitBox.bottom + m_HitBox.height - playerHitBox.height / 2;
        if (player.IsDashing())
            playerVelocity.y *= -1;
        else
            playerVelocity.y *= -player.GetBouncinessWalls();
    }


    player.SetVelocity(playerVelocity);


}


