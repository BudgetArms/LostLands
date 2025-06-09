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

    Point2f GetPosition() const { return m_Position; };
    Vector2f GetVelocity() const { return m_Velocity; };
    void SetPosition(Point2f position) { m_Position = position; };
    void SetVelocity(Vector2f velocity) { m_Velocity = velocity; };

    float GetHealth() const { return m_Health; };
    void SetHealth(float health);

    float GetSpeed() const { return m_Speed; };
    void SetSpeed(float speed) { m_Speed = speed; };

    void SetShootingEnabled(bool ableToShoot) { m_bIsShootingEnabled = ableToShoot; };

    float GetBulletsPerSecond() const { return m_BulletsPerSecond; };
    void SetBulletsPerSecond(float bulletsPerSecond) { m_BulletsPerSecond = bulletsPerSecond; };

    bool IsDead() const { return m_bIsDead; };
    bool IsMarkedForDeletion() const { return m_bIsMarkedForDeletion; };


private:
    float m_Health{};


protected:
    virtual void Shoot() {};


    Point2f m_Position;
    Vector2f m_Velocity{};
    Vector2f m_Direction{ 1.f, 0.f };
    Color4f m_Color{};
    Color4f m_HealthBarColor{};

    Rectf   m_HitBox{};

    float m_Width{};
    float m_Height{};

    float m_Speed{ 200.f };
    float m_MaxHealth{};


    bool m_bIsShootingEnabled{};
    bool m_bIsWalkingEnabled{};
    bool m_bIsDashingEnabled{};
    bool m_bIsMirroringEnabled{};
    bool m_bCanWalk{};
    bool m_bCanShoot{};
    bool m_bIsDead{};

    bool m_bIsMarkedForDeletion{ false };

    const float m_DestroyDelayTime{ 0.5f };
    float m_DestroyDelayCooldown{};

    float m_BulletsPerSecond{ 2.f };
    float m_AccumulatedSecBullets{};


};


