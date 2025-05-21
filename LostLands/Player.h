#pragma once
#include <unordered_map>

#include "Character.h"


class Player final : public Character
{
public:

    Player(const Point2f& position);

    Player(Player&) = delete;	// Copy constructor
    Player(Player&&) = delete;	// Move constructor
    Player operator&=(Player&) = delete;	// Copy Assignment
    Player operator&=(Player&&) = delete;	// Move Assignment

    ~Player();

    virtual void Draw() const override;
    virtual void Update(float elapsedSec) override;

    virtual Rectf& GetHitBox() override;


    virtual void Damage(float damage) override;
    virtual void Shoot() override;



private:
    void HandleKeyboardInput(float elapsedSec);
    void HandleMouseInput(float elapsedSec);
    void Friction(float elapsedSec);
    void Dash();

    void HandleWallCollisions();


    Vector2f m_Input{};

    float m_Acceleration{ 300.f };
    float m_Deceleration{ 100.f };

    // dash
    const float m_DashSpeed{ 300.f };
    const float m_DashTime{ 1.f };
    float m_DashTimer{ 0 };
    const float m_DashCooldown{ 5.f };


    const float m_IFramesTime{ 2.f };
    float m_IFramesCountDown{};


    const float m_BouncinessWalls{ 2 };

    bool m_bIsDashing{};


protected:


};


