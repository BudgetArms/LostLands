#pragma once


class Character
{
public:
    Character(const Point2f& position = {});
    virtual ~Character() = default;

    Character(Character& other) = delete;
    Character(Character&& other) = delete;
    Character& operator=(Character& other) = delete;
    Character& operator=(Character&& other) = delete;


    virtual void Draw() const = 0;
    virtual void Update(float elapsedSec);
    virtual void Damage(float health);


    virtual Rectf& GetHitBox() = 0;

    float GetHealth() const { return m_Health; };
    void SetHealth(float health);

    bool IsDead() const { return m_bIsDead; };
    bool IsMarkedForDeletion() const { return m_bIsMarkedForDeletion; };


    Point2f m_Position;
    Vector2f m_Velocity{};
    Vector2f m_Direction{ 1.f, 0.f };
    Color4f m_Color{};
    Color4f m_HealthBarColor{};

    float m_Speed{ 200.f };

    bool m_bIsShootingEnabled{};
    float m_BulletsPerSecond{ 2.f };

private:
    float m_Health{};


protected:
    virtual void Shoot() {};


    Rectf   m_HitBox{};

    float m_Width{};
    float m_Height{};

    float m_MaxHealth{};


    bool m_bIsWalkingEnabled{};
    bool m_bIsDashingEnabled{};
    bool m_bIsMirroringEnabled{};
    bool m_bCanWalk{};
    bool m_bCanShoot{};
    bool m_bIsDead{};

    bool m_bIsMarkedForDeletion{ false };

    const float m_DestroyDelayTime{ 0.5f };
    float m_DestroyDelayCooldown{};

    float m_AccumulatedSecBullets{};


};


