#pragma once

class Character;

class SpeedPad final
{
public:
    explicit SpeedPad(const Rectf& area, const Vector2f& direction, float speedBoost);
    ~SpeedPad() = default;

    void Draw() const;

    void OnCollision(Character* character);


    Point2f GetPosition() const { return Point2f(m_HitBox.left, m_HitBox.bottom); }
    Rectf GetHitBox() const { return m_HitBox; }

    bool GetActive() const { return m_bIsActive; }
    void SetActive(bool active) { m_bIsActive = active; }

    const Vector2f& GetDirection() const { return m_Direction; }
    void SetDirection(const Vector2f& direction) { m_Direction = direction.Normalized(); }

    float GetSpeedBoost() const { return m_SpeedBoost; }
    void GetSpeedBoost(float boost) { m_SpeedBoost = boost; }


private:
    Rectf m_HitBox;
    Vector2f m_Direction;
    float m_SpeedBoost;
    bool m_bIsActive;


};


