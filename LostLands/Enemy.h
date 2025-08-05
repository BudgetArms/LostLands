#pragma once
#include "Character.h"


class Enemy : public Character
{
public:
	Enemy(const Point2f& position);


	bool m_bIsEnabled{ true };


private:
	void Draw() const override;
	void Update(float elapsedSec) override;

	Rectf& GetHitBox() override;
	virtual void Shoot() override;


};


