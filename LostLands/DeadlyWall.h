#pragma once
#include "Wall.h"


class DeadlyWall : public Wall
{
public:
	DeadlyWall(const Rectf& area, float damage);


	virtual void Draw() const override;

	virtual void HandleCollisionsPlayer(Player& player) override;
	virtual void HandleCollisionsBullet(Bullet& bullet) override;


private:
	float m_Damage;


};


