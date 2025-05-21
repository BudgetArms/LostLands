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

    void Mirror();

    void SetDashingEnabled(bool ableToDash) { m_bIsDashingEnabled = ableToDash; };
    void SetMirroringEnabled(bool ableToMirror) { m_bIsMirroringEnabled = ableToMirror; };


private:
    void HandleKeyboardInput(float elapsedSec);
    void HandleMouseInput(float elapsedSec);
    void Friction(float elapsedSec);
    void Dash();

    void HandleWallCollisions();


    Vector2f m_Input{};

    float m_Acceleration{ 300.f };
    float m_Deceleration{ 100.f };

    bool m_bIsDashingEnabled{};
    bool m_bIsMirroringEnabled{};



    // dash
    bool m_bIsDashing{};
    const float m_DashSpeed{ 300.f };
    const float m_DashTime{ 1.f };
    float m_DashTimer{};
    const float m_DashCooldown{ 5.f };

    // mirror
    bool m_bCanMirror{};
    float m_MirrorTimer{};
    const float m_MirrorCooldown{ 1.f };


    const float m_IFramesTime{ 2.f };
    float m_IFramesCountDown{};
    const float m_BouncinessWalls{ 2 };



protected:


};


