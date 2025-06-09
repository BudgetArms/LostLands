#pragma once
#include "Wall.h"


class DeadlyWall final : public Wall
{
public:
    DeadlyWall(const Rectf& area, float damage);

    virtual void Draw() const override;
    virtual void HandleCollisions(Player& player) override;


private:
    float m_Damage;


};


