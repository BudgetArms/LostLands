#include "pch.h"
#include "Heart.h"


Heart::Heart(const Point2f& position, float width) :
	m_Position{ position },
	m_Width{ width }
{
}


void Heart::Draw() const
{
	// draw heart
	utils::SetColor(0.7f, 0.f, 0.f, 1.f);

	glTranslatef(m_Position.x, m_Position.y, 0);
	glScalef(m_Width / 1000.f, m_Width / 1000.f, 0.f);

	// first rect
	glRotatef(45.f, 0, 0, 1);
	utils::FillRect(0, 0, 585, 530);
	glRotatef(-45, 0, 0, 1);

	// second rect
	glScalef(-1, 1, 1);
	glRotatef(45.f, 0, 0, 1);
	utils::FillRect(0, 0, 585, 530);
	glRotatef(-45.f, 0, 0, 1);
	glScalef(-1, 1, 1);

	// Circles
	utils::FillEllipse(220, 605, 270, 270);
	utils::FillEllipse(-220, 605, 270, 270);

	glScalef(1000.f / m_Width, 1000.f / m_Width, 0.f);
	glTranslatef(-m_Position.x, -m_Position.y, 0);
}



