#pragma once

#include "Wall.h"


class UnlockWall : public Wall
{
public:
	UnlockWall(const Rectf& rect);


	void Draw() const override;


private:
	Color4f m_Color{ 0.7f, 0.7f, 0.7f, 1.f };


};


