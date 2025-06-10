#pragma once
#include "utils.h"

class Player;
class Bullet;

class Wall
{
public:
    explicit Wall(const Rectf& area);
    ~Wall() = default;

    virtual void Draw() const;

    virtual void HandleCollisions(Player& character);
    virtual void HandleCollisionsBullet(Bullet& bullet);

    Point2f GetPosition() const { return Point2f(m_Area.left, m_Area.bottom); }
    virtual Rectf GetArea() const { return m_Area; }


private:


protected:
    Rectf m_Area;


};


