#pragma once
#include "utils.h"

class Player;
class Bullet;

class Wall
{
public:
	explicit Wall(const Rectf& area);
	virtual ~Wall();

	virtual void Draw() const;

	virtual void HandleCollisionsPlayer(Player& character);
	virtual void HandleCollisionsBullet(Bullet& bullet);

	Point2f GetPosition() const { return Point2f(m_Area.left, m_Area.bottom); }
	virtual Rectf GetArea() const { return m_Area; }


	bool m_bRenderWall{ true };


protected:
	void HandleCollisions(const Rectf& hitbox, Point2f& outPosition, Vector2f& outVelocity, float bouncinessWalls);


	Rectf m_Area;
	const float m_OffsetAfterCollisions{ 2.f };


};


