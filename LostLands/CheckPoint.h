#pragma once

#include "EntityManager.h"


class CheckPoint
{
public:
	CheckPoint(const Rectf& area);

	void Draw() const;

	const Rectf& GetArea() const;
	void SetColor(const Color4f& color);


private:
	Rectf m_Area;
	Color4f m_Color{ 0.f, 0.3f, 0.12f, 1.f };


};


