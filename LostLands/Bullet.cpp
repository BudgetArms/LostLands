#include "pch.h"
#include "Bullet.h"



Bullet::Bullet(BulletType characterType, const Point2f& position, float angleDirection, float speed) :
    m_BulletType{ characterType },
    m_Position{ position },
    m_HitBox{},
    m_Direction{ cosf(utils::g_toRadians * angleDirection), sinf(utils::g_toRadians * angleDirection) },
    m_Speed{ speed }
{
    m_HitBox = Rectf(m_Position.x - m_Size / 2, m_Position.y - m_Size / 2, m_Size, m_Size);
}



void Bullet::Draw() const
{
    switch (m_BulletType)
    {
    case BulletType::Player:
        utils::SetColor(0.4f, 0.2f, 0.8f);
        break;
    case BulletType::Enemy:
        utils::SetColor(0.9f, 0.2f, 0.2f);
        break;
    }

    utils::FillEllipse(m_Position, m_Size / 2, m_Size / 2);

    if (g_bShowHitboxes)
    {
        auto hitbox = Rectf(m_Position.x - m_Size / 2, m_Position.y - m_Size / 2, m_Size, m_Size);
        utils::SetColor(g_HitBoxesColor);
        utils::DrawRect(hitbox);
    }
}



void Bullet::Update(float elapsedSec)
{
    m_LifeTime -= elapsedSec;

    if (m_LifeTime <= -1.1f)
    {
        Destroy();
        return;
    }
    else if (m_LifeTime <= 0.f)
        m_Size /= (1.f + 2.f * elapsedSec);

    m_Position += m_Direction * m_Speed * elapsedSec * 0.8f;
    HandleWallCollisions();
}

Rectf& Bullet::GetHitBox()
{
    m_HitBox.left = m_Position.x - m_Size / 2;
    m_HitBox.bottom = m_Position.y - m_Size / 2;
    return m_HitBox;
}

void Bullet::HandleWallCollisions()
{
    GetHitBox();

    if (m_HitBox.left <= g_SmallWindow.left)
    {
        // hit left wall
        m_Position.x = g_SmallWindow.left + m_Size / 2;
        m_Direction.x *= -1;
    }
    else if (m_HitBox.left + m_HitBox.width >= g_SmallWindow.left + g_SmallWindow.width)
    {
        // hit right wall
        m_Position.x = g_SmallWindow.left + g_SmallWindow.width - m_Size / 2;
        m_Direction.x *= -1;
    }
    else if (m_HitBox.bottom <= g_SmallWindow.bottom)
    {
        // hit bottom wall
        m_Position.y = g_SmallWindow.bottom + m_Size / 2;
        m_Direction.y *= -1;
    }
    else if (m_HitBox.bottom + m_HitBox.height >= g_SmallWindow.bottom + g_SmallWindow.height)
    {
        // hit top wall
        m_Position.y = g_SmallWindow.bottom + g_SmallWindow.height - m_Size / 2;
        m_Direction.y *= -1;
    }

}


