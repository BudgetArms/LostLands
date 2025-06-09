#pragma once

#include "EntityManager.h"


class Character;

class Bullet final
{
public:
    Bullet(BulletType characterType, const Point2f& position, float angleDirection, float speed = 400.f);
    ~Bullet() = default;

    Bullet(const Bullet& other) = delete;
    Bullet(Bullet&& other) = delete;
    Bullet operator&=(const Bullet& other) = delete;
    Bullet operator&=(Bullet&& other) = delete;


    void Draw() const;
    void Update(float elapsedSec);

    void Destroy() { m_bMarkedForDeletion = true; };


    Rectf& GetHitBox();

    float GetDamage() const { return m_Damage; };
    void SetDamage(float damage) { m_Damage = damage; };

    BulletType GetType() const { return m_BulletType; };

    Point2f GetPosition() const { return m_Position; };
    void SetPosition(Point2f position) { m_Position = position; };

    Vector2f GetDirection() const { return m_Direction; };
    void SetDirection(Vector2f direction) { m_Direction = direction; };

    float GetSpeed() const { return m_Speed; };
    void SetSpeed(float speed) { m_Speed = speed; };


    void SetLifeTime(float lifetime) { m_LifeTime = lifetime; };
    bool IsMarkedForDeletion() const { return m_bMarkedForDeletion; };


private:
    void HandleWallCollisions();


    BulletType m_BulletType;
    Point2f m_Position;

    Rectf m_HitBox;
    Vector2f m_Direction;
    float m_Speed;

    float m_Damage{ 30.f };
    float m_LifeTime{ 3.f };

    float m_Size{ 20.f };

    bool m_bMarkedForDeletion{ false };


};


