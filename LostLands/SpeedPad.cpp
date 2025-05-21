#include "pch.h"
#include "SpeedPad.h"

#include "Character.h"


SpeedPad::SpeedPad(const Rectf& area, const Vector2f& direction, float speedBoost) :
    m_HitBox{ area },
    m_Direction{ direction.Normalized() },
    m_SpeedBoost{ speedBoost },
    m_bIsActive{ true }
{
}

void SpeedPad::Draw() const
{
    // Draw the speed pad background
    utils::SetColor(1, 0, 0, 0.1f);
    utils::FillRect(m_HitBox);

    // Calculate center point of the hitbox
    float centerX = m_HitBox.left + m_HitBox.width / 2.0f;
    float centerY = m_HitBox.bottom + m_HitBox.height / 2.0f;

    // Calculate direction angle
    float angle = atan2(m_Direction.y, m_Direction.x);

    // Arrow parameters
    float arrowLength = 30.0f;
    float lineThickness = 2.0f;
    float arrowHeadLength = 10.0f;
    float arrowHeadWidth = 6.0f;

    // Calculate endpoint of the arrow line
    float endX = centerX + m_Direction.x * (arrowLength / 2.0f);
    float endY = centerY + m_Direction.y * (arrowLength / 2.0f);

    // Calculate start point of the arrow line
    float startX = centerX - m_Direction.x * (arrowLength / 2.0f);
    float startY = centerY - m_Direction.y * (arrowLength / 2.0f);

    // Draw arrow line using a rotated rectangle
    utils::SetColor(0, 1, 0, 1);

    // For a properly aligned line, we need to transform our rectangle
    // Create line points
    Point2f p1(startX, startY);
    Point2f p2(endX, endY);

    // Direction vector from start to end
    Vector2f lineDir(p2.x - p1.x, p2.y - p1.y);
    float lineLength = lineDir.Length();

    // Calculate perpendicular vector for line thickness
    Vector2f perpDir(-lineDir.y / lineLength, lineDir.x / lineLength);
    perpDir *= lineThickness / 2.0f;

    // Create the four corners of the line rectangle
    Point2f lineCorners[4];
    lineCorners[0] = Point2f(p1.x + perpDir.x, p1.y + perpDir.y);
    lineCorners[1] = Point2f(p2.x + perpDir.x, p2.y + perpDir.y);
    lineCorners[2] = Point2f(p2.x - perpDir.x, p2.y - perpDir.y);
    lineCorners[3] = Point2f(p1.x - perpDir.x, p1.y - perpDir.y);

    // Draw the line as a quad
    utils::FillPolygon(lineCorners, 4);

    // Calculate arrowhead points
    Point2f arrowTip(endX, endY);

    Point2f arrowP1(
        endX - arrowHeadLength * cos(angle) + arrowHeadWidth * sin(angle),
        endY - arrowHeadLength * sin(angle) - arrowHeadWidth * cos(angle)
    );

    Point2f arrowP2(
        endX - arrowHeadLength * cos(angle) - arrowHeadWidth * sin(angle),
        endY - arrowHeadLength * sin(angle) + arrowHeadWidth * cos(angle)
    );

    // Draw arrowhead
    utils::FillTriangle(arrowTip, arrowP1, arrowP2);
}

void SpeedPad::OnCollision(Character* character)
{
    if (!m_bIsActive)
        return;

    Vector2f boost{ m_Direction.x * m_SpeedBoost, m_Direction.y * m_SpeedBoost };
    boost += character->GetVelocity();

    character->SetVelocity(boost);
    // Character->ApplyForce(boost);

}


