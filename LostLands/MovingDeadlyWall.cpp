#include "pch.h"
#include "MovingDeadlyWall.h"

#include <cmath>


MovingDeadlyWall::MovingDeadlyWall(const Rectf& area, float damage,
    const Vector2f& direction, float speed, float range) :
    DeadlyWall(area, damage),
    m_Offset{ },
    m_Direction{ direction.Normalized() },
    m_Speed{ speed },
    m_Range{ range },
    m_Angle{ }
{
}

void MovingDeadlyWall::Update(float elapsedSec)
{
    m_Angle += elapsedSec;
    const float offset{ std::cosf(m_Angle * m_Speed) * m_Range / 2.f };
    m_Offset = Point2f(offset * m_Direction);
}

Rectf MovingDeadlyWall::GetArea() const
{
    return Rectf(m_Area.left - m_Offset.x, m_Area.bottom - m_Offset.y, m_Area.width, m_Area.height);
}


