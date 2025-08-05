#pragma once

#include "UnlockWall.h"


class UnlockArea final
{
public:
	UnlockArea(const Rectf& area, UnlockWall* pUnlockWall);
	~UnlockArea() = default;

	void Draw() const;


	const Rectf& GetArea() const { return m_Area; };
	UnlockWall* GetWall() { return m_pUnlockWall; }


private:
	Color4f m_Color{ 0.f, 1.f, 1.f, 0.7f };
	Rectf m_Area;
	UnlockWall* m_pUnlockWall;


};


