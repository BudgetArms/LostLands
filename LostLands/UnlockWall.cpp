#include "pch.h"
#include "UnlockWall.h"



UnlockWall::UnlockWall(const Rectf& area) :
	Wall(area)
{
}

void UnlockWall::Draw() const
{
	utils::SetColor(m_Color);
	utils::FillRect(m_Area);
}


