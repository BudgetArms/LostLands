#pragma once


class Character;

class SpeedPad final
{
public:
    explicit SpeedPad(const Rectf& area, const Vector2f& direction, float speedBoost);
    ~SpeedPad() = default;


    void Draw() const;
    void OnCollision(Character* character, float elapsedSec);

    Point2f GetPosition() const { return Point2f(m_HitBox.left, m_HitBox.bottom); }
    Rectf GetHitBox() const { return m_HitBox; }


    Vector2f m_Direction;
    float m_SpeedBoost;
    bool m_bIsActive;


private:
    Rectf m_HitBox;


};


