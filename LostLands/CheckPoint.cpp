#include "pch.h"

#include "CheckPoint.h"


CheckPoint::CheckPoint(const Rectf& area) :
	m_Area{ area }
{
}

void CheckPoint::Draw() const
{
	utils::SetColor(m_Color);
	utils::FillRect(m_Area);
}


const Rectf& CheckPoint::GetArea() const
{
	return m_Area;
}

void CheckPoint::SetColor(const Color4f& color)
{
	m_Color = color;
}

