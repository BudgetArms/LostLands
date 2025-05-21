#pragma once

class Player;

class Wall final
{
public:
    explicit Wall(const Rectf& area);
    ~Wall() = default;

    void Draw() const;

    void HandleCollisions(Player& character);

    Point2f GetPosition() const { return Point2f(m_HitBox.left, m_HitBox.bottom); }
    Rectf GetHitBox() const { return m_HitBox; }

    bool GetActive() const { return m_bIsActive; }
    void SetActive(bool active) { m_bIsActive = active; }


private:
    Rectf m_HitBox;
    bool m_bIsActive;


};


