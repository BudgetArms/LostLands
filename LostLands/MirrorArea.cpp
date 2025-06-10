#include "pch.h"
#include "MirrorArea.h"


MirrorArea::MirrorArea(const Rectf& area) :
    m_Area{ area }
{
}

void MirrorArea::Draw() const
{
    utils::SetColor(0, 0, 0.6f, 1.f);
    utils::FillRect(m_Area);
}

bool MirrorArea::CanMirrorTo(const Point2f& position) const
{
    return utils::IsPointInRect(position, m_Area);
}



