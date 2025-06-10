#pragma once
#include "DeadlyWall.h"
class MovingDeadlyWall : public DeadlyWall
{
public:
    MovingDeadlyWall(const Rectf& area, float damage, const Vector2f& direction, float speed, float range);
    ~MovingDeadlyWall() = default;

    //void Draw() const;
    void Update(float elapsedSec);

    virtual Rectf GetArea() const override;


private:
    Point2f m_Offset;
    const Vector2f m_Direction;
    const float m_Speed;
    const float m_Range;

    float m_Angle;


};


