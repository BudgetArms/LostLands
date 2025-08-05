#pragma once

#include "Enemy.h"


class TurretEnemy : public Enemy
{
public:
	TurretEnemy(const Point2f& position, const Vector2f& direction);

	void Draw() const override;
	void Shoot() override;


private:
	Vector2f m_Direction;


};


