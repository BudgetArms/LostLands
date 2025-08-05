#include "pch.h"
#include "UnlockArea.h"


UnlockArea::UnlockArea(const Rectf& area, UnlockWall* pUnlockWall) :
	m_Area{ area },
	m_pUnlockWall{ pUnlockWall }
{
}



void UnlockArea::Draw() const
{
	utils::SetColor(m_Color);
	utils::FillRect(m_Area);
}



