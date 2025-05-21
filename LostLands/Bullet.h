#pragma once


class Character;

class Bullet final
{
public:
    Bullet(Character& owner, const Point2f& position, float angleDirection, float speed = 400.f);
    ~Bullet() = default;

    Bullet(const Bullet& other) = delete;
    Bullet(Bullet&& other) = delete;
    Bullet operator&=(const Bullet& other) = delete;
    Bullet operator&=(Bullet&& other) = delete;


    void Draw() const;
    void Update(float elapsedSec);

    Rectf& GetHitBox();
    float GetDamage() const { return m_Damage; };
    Character* GetOwner() const { return &m_Owner; };


    void SetLifeTime(float lifetime);
    void SetDamage(float damage);

    void Destroy();

    bool IsMarkedForDeletion() const { return m_bMarkedForDeletion; };

private:
    void HandleWallCollisions();

    Character& m_Owner;
    Point2f m_Position;

    Rectf m_HitBox{};
    Vector2f m_Direction;
    const float m_Speed;

    float m_Damage{ 30.f };
    float m_LifeTime{ 3.f };

    float m_Size{ 20.f };

    bool m_bMarkedForDeletion{ false };


protected:


};


