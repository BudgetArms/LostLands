#include "pch.h"
#include "SpeedPad.h"

#include "Character.h"


SpeedPad::SpeedPad(const Rectf& area, const Vector2f& direction, float speedBoost) :
	m_Direction{ direction.Normalized() },
	m_SpeedBoost{ speedBoost },
	m_bIsActive{ true },
	m_HitBox{ area }
{
}

void SpeedPad::Draw() const
{
	// Draw the speed pad background
	utils::SetColor(m_ColorBackground);
	utils::FillRect(m_HitBox);

	const float centerX = m_HitBox.left + m_HitBox.width / 2.0f;
	const float centerY = m_HitBox.bottom + m_HitBox.height / 2.0f;

	const float angle = atan2(m_Direction.y, m_Direction.x);

	const float arrowLength = 30.0f;
	const float lineThickness = 2.0f;
	const float arrowHeadLength = 10.0f;
	const float arrowHeadWidth = 6.0f;

	const float endX = centerX + m_Direction.x * (arrowLength / 2.0f);
	const float endY = centerY + m_Direction.y * (arrowLength / 2.0f);

	const float startX = centerX - m_Direction.x * (arrowLength / 2.0f);
	const float startY = centerY - m_Direction.y * (arrowLength / 2.0f);

	utils::SetColor(m_ColorArrow);

	const Point2f p1(startX, startY);
	const Point2f p2(endX, endY);

	const Vector2f lineDir(p2.x - p1.x, p2.y - p1.y);
	float lineLength = lineDir.Length();

	Vector2f perpDir(-lineDir.y / lineLength, lineDir.x / lineLength);
	perpDir *= lineThickness / 2.0f;

	Point2f lineCorners[4];
	lineCorners[0] = Point2f(p1.x + perpDir.x, p1.y + perpDir.y);
	lineCorners[1] = Point2f(p2.x + perpDir.x, p2.y + perpDir.y);
	lineCorners[2] = Point2f(p2.x - perpDir.x, p2.y - perpDir.y);
	lineCorners[3] = Point2f(p1.x - perpDir.x, p1.y - perpDir.y);

	utils::FillPolygon(lineCorners, 4);

	const Point2f arrowTip(endX, endY);

	const Point2f arrowP1(
		endX - arrowHeadLength * cos(angle) + arrowHeadWidth * sin(angle),
		endY - arrowHeadLength * sin(angle) - arrowHeadWidth * cos(angle)
	);

	const Point2f arrowP2(
		endX - arrowHeadLength * cos(angle) - arrowHeadWidth * sin(angle),
		endY - arrowHeadLength * sin(angle) + arrowHeadWidth * cos(angle)
	);

	utils::FillTriangle(arrowTip, arrowP1, arrowP2);
}

void SpeedPad::OnCollision(Character* character, float elapsedSec)
{
	if (!m_bIsActive)
		return;

	Vector2f boost{ m_Direction.x * m_SpeedBoost, m_Direction.y * m_SpeedBoost };
	boost *= elapsedSec;
	boost += character->m_Velocity;

	character->m_Velocity = boost;
}


