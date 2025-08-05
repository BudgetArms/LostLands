#pragma once
#include <unordered_map>

#include "Character.h"
#include "Heart.h"


class Player final : public Character
{
public:

	Player(const Point2f& position, int lives);


	virtual void Draw() const override;
	virtual void Update(float elapsedSec) override;

	virtual Rectf& GetHitBox() override;

	void Reset();
	virtual void Damage(float damage) override;
	virtual void Shoot() override;

	void Mirror();

	void SetDashingEnabled(bool ableToDash) { m_bIsDashingEnabled = ableToDash; };
	void SetMirroringEnabled(bool ableToMirror) { m_bIsMirroringEnabled = ableToMirror; };

	float GetBouncinessWalls() const { return m_BouncinessWalls; };
	void SetBouncinessWalls(float bouncinessWalls) { m_BouncinessWalls = bouncinessWalls; };

	bool IsDashing() const { return m_bIsDashing; };

	void SetDashCoolDown(float newCooldownTime) { m_DashCooldown = newCooldownTime; };


	float m_DashTime{ 1.f };


private:
	void HandleKeyboardInput(float elapsedSec);
	void HandleMouseInput(float elapsedSec);
	void Friction(float elapsedSec);
	void Dash();

	void HandleWallCollisions();


	const Point2f m_HeartStartPosition{ 50.f, 2.f };
	const float m_HeartWidth{ 30.f };
	float m_HeartOffset{ 30.f };
	std::vector<Heart> m_Hearts{};


	Vector2f m_Input{};

	float m_MaxVelocity{ 500 };

	float m_Acceleration{ 300.f };
	float m_Deceleration{ 100.f };

	int m_Lives;

	bool m_bIsDashingEnabled{};
	bool m_bIsMirroringEnabled{};

	// dash
	bool m_bIsDashing{};
	bool m_bCanMirror{};

	const float m_DashSpeed{ 300.f };
	float m_DashTimer{};
	float m_DashCooldown{ 2.f };

	// mirror
	float m_MirrorTimer{};
	const float m_MirrorCooldown{ 2.f };

	// IFrames
	const float m_IFramesTime{ 0.5f };
	float m_IFramesCountDown{};

	float m_BouncinessWalls{ 2.f };


};


