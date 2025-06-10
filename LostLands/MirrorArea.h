#pragma once


class MirrorArea final
{
public:
    MirrorArea(const Rectf& area);
    ~MirrorArea() = default;


    void Draw() const;
    bool CanMirrorTo(const Point2f& position) const;


    Rectf m_Area;


};


