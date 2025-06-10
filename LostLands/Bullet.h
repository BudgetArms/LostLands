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

    BulletType GetType() const { return m_BulletType; };

    bool IsMarkedForDeletion() const { return m_bMarkedForDeletion; };


    Point2f m_Position;
    Vector2f m_Direction;

    float m_Speed;
    float m_Damage{ 30.f };
    float m_LifeTime{ 2.f };


private:
    void HandleWallCollisions();


    BulletType m_BulletType;
    Rectf m_HitBox{};

    float m_Size{ 20.f };
    bool m_bMarkedForDeletion{ false };


};


