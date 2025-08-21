#include "pch.h"
#include "Player.h"

#include <algorithm>

#include "InputManager.h"
#include "EntityManager.h"
#include "LevelManager.h"

#include "Bullet.h"
#include "CheckPoint.h"
#include "MirrorArea.h"
#include "Utils.h"


Player::Player(const Point2f& position, int lives) :
	Character(position),
	m_Lives{ lives }
{
	for (int i{}; i < m_Lives; ++i)
		m_Hearts.emplace_back(Point2f(m_HeartStartPosition.x + i * m_HeartOffset, m_HeartStartPosition.y), m_HeartWidth);

	m_Color = Color4f(1.f, 1.f, 1.f, 1.f);
	m_HealthBarColor = Color4f(0.f, 1.f, 0.f, 0.8f);

	m_Width = 20.f;
	m_Height = 20.f;

	m_Speed = 150.f;
	m_MaxHealth = 100.f;
	SetHealth(m_MaxHealth);
	m_bIsWalkingEnabled = true;
	m_bCanWalk = true;

	m_HitBox.left = m_Position.x;
	m_HitBox.bottom = m_Position.y;
	m_HitBox.width = m_Width;
	m_HitBox.height = m_Height;



}

void Player::Draw() const
{
	// draw Player
	utils::SetColor(m_Color);
	utils::FillEllipse(m_Position, m_Width / 2, m_Height / 2);

	// draw HealthBar
	constexpr float healthBarOffset{ 20.f };
	utils::SetColor(m_HealthBarColor);
	utils::FillRect(m_Position.x - healthBarOffset, m_Position.y + healthBarOffset,
		(GetHealth() / m_MaxHealth) * (2 * healthBarOffset), 10.f);

	utils::SetColor(1.f, 1.f, 1.f, 1.f);
	utils::DrawRect(m_Position.x - healthBarOffset, m_Position.y + healthBarOffset,
		(2 * healthBarOffset), 10.f, 2.f);

	if (g_bShowHitboxes)
	{
		Rectf hitbox{ m_Position.x - m_Width / 2, m_Position.y - m_Height / 2, m_Width, m_Height };
		utils::SetColor(g_HitBoxesColor);
		utils::DrawRect(hitbox);
	}

	constexpr float heightOffset{ 25.f };
	constexpr float height{ 15.f };
	const float dashX{ 90.f };
	const float mirrorX{ 275.f };

	// draw dash cooldown
	utils::SetColor(0.f, 0.f, 1.f, 1.f);
	utils::FillRect(dashX, g_Window.height - heightOffset, 100 * (1 - std::clamp<float>((m_DashTimer / m_DashCooldown), 0, 1)), height);

	utils::SetColor(0.f, 0.f, 0.f, 1.f);
	utils::DrawRect(dashX, g_Window.height - heightOffset, 100, height, 2.f);

	// draw teleport cooldown
	utils::SetColor(0.f, 0.f, 1.f, 1.f);
	utils::FillRect(mirrorX, g_Window.height - heightOffset, 100 * (1 - std::clamp<float>((m_MirrorTimer / m_MirrorCooldown), 0, 1)), height);

	utils::SetColor(0.f, 0.f, 0.f, 1.f);
	utils::DrawRect(mirrorX, g_Window.height - heightOffset, 100, height, 2.f);


	for (const auto& heart : m_Hearts)
		heart.Draw();


}

void Player::Update(float elapsedSec)
{
	Character::Update(elapsedSec);

	if (m_bIsDead)
		return;

	if (m_IFramesCountDown >= 0.0f)
		m_IFramesCountDown -= elapsedSec;

	HandleKeyboardInput(elapsedSec);
	HandleMouseInput(elapsedSec);


	m_Velocity.x = std::clamp<float>(m_Velocity.x, -m_MaxVelocity, m_MaxVelocity);
	m_Velocity.y = std::clamp<float>(m_Velocity.y, -m_MaxVelocity, m_MaxVelocity);

	if (m_bIsMirroringEnabled)
	{
		m_MirrorTimer -= elapsedSec;
		if (m_MirrorTimer <= 0)
			m_bCanMirror = true;
	}

}

Rectf& Player::GetHitBox()
{
	m_HitBox.left = m_Position.x - m_Width / 2;
	m_HitBox.bottom = m_Position.y - m_Height / 2;

	return m_HitBox;
}

void Player::Reset()
{
	SetHealth(m_MaxHealth);
	if (auto pCheckPoint = LevelManager::GetInstance().m_CurrentCheckPoint)
	{
		const auto& area = pCheckPoint->GetArea();
		m_Position = Point2f(area.left + area.width / 2, area.bottom + area.height / 2);
	}
	else
	{
		m_Position = LevelManager::GetInstance().m_CurrentSpawnPosition;
	}

	m_Velocity = {};

}

void Player::Damage(float damage)
{
	if (m_IFramesCountDown > 0)
		return;

	Character::Damage(damage);

	m_IFramesCountDown = m_IFramesTime;

	if (m_bIsDead)
	{
		--m_Lives;

		if (!m_Hearts.empty())
			m_Hearts.pop_back();

		if (m_Lives > 0)
		{
			m_bIsDead = false;
			Reset();
		}
	}
}

void Player::Shoot()
{
	if (!m_bCanShoot || !m_bIsShootingEnabled)
		return;

	int x{}, y{};
	SDL_GetMouseState(&x, &y);

	y = -y + int(g_Window.height);
	const float directionAngle{ utils::g_toDegrees * atan2f(y - m_Position.y, x - m_Position.x) };

	EntityManager::GetInstance().SpawnBullet(BulletType::Player, m_Position, directionAngle);
	m_bCanShoot = false;
}

void Player::Mirror()
{
	if (!m_bCanMirror || !m_bIsMirroringEnabled)
		return;

	const Point2f mirrorPosition{ g_Window.width - m_Position.x, g_Window.height - m_Position.y };

	if (auto& mirrorAreas = EntityManager::GetInstance().GetMirrorAreas(); !mirrorAreas.empty())
	{
		bool canMirror{ false };
		for (auto& mirrorArea : mirrorAreas)
			if (mirrorArea.CanMirrorTo(mirrorPosition))
			{
				canMirror = true;
				break;
			}

		if (!canMirror)
			return;

	}

	m_Position = mirrorPosition;
	m_MirrorTimer = m_MirrorCooldown;

	m_bCanMirror = false;
}

void Player::HandleKeyboardInput(float elapsedSec)
{
	m_Input = {};

	// get direction
	const auto& keysDownMap = InputManager::GetInstance().GetKeysDownMap();
	for (const auto& pair : keysDownMap)
	{
		if (pair.second == false)
			continue;

		if (pair.first == SDLK_w)
			m_Input.y += 1;

		if (pair.first == SDLK_s)
			m_Input.y -= 1;

		if (pair.first == SDLK_d)
			m_Input.x += 1;

		if (pair.first == SDLK_a)
			m_Input.x -= 1;

	}

	if (auto it = keysDownMap.find(SDLK_SPACE); it != keysDownMap.end())
		if (it->second && m_DashTimer <= 0 && !m_bIsDashing)
			Dash();


	m_Input = m_Input.Normalized();


	m_DashTimer -= elapsedSec;
	if (m_bIsDashing)
	{
		if (m_DashTimer < m_DashCooldown)
			m_bIsDashing = false;
	}
	else
	{
		if (m_Input.Length() != 0.f)
		{
			m_Velocity += m_Input * m_Acceleration * elapsedSec;

			if (m_Velocity.Norm() > m_Speed)
				m_Velocity = m_Speed * m_Velocity.Normalized();

			m_Direction = m_Velocity.Normalized();
		}
		else
			Friction(elapsedSec);

	}

	m_Position += m_Velocity * elapsedSec;
}

void Player::HandleMouseInput(float elapsedSec)
{
	for (const auto& pair : InputManager::GetInstance().GetMouseDownMap())
	{
		if (pair.second == false)
			continue;

		if (pair.first == SDL_BUTTON_LEFT)
			Shoot();

		if (pair.first == SDL_BUTTON_RIGHT)
			Mirror();

	}
}

void Player::Friction(float elapsedSec)
{
	if (m_Velocity.Length() == 0)
		return;

	if (m_Velocity.x != 0)
	{
		float sign = (m_Velocity.x > 0) ? 1.0f : -1.0f;
		m_Velocity.x -= sign * m_Deceleration * elapsedSec;

		if (sign * m_Velocity.x < 0)
			m_Velocity.x = 0;
	}

	if (m_Velocity.y != 0)
	{
		float sign = (m_Velocity.y > 0) ? 1.0f : -1.0f;
		m_Velocity.y -= sign * m_Deceleration * elapsedSec;

		if (sign * m_Velocity.y < 0)
			m_Velocity.y = 0;
	}
}

void Player::Dash()
{
	m_bIsDashing = true;
	m_DashTimer = m_DashTime + m_DashCooldown;

	// use input, if there is input
	// else use the last input
	if (m_Input.Length() != 0)
	{
		m_Velocity = m_DashSpeed * m_Input;
	}
	else
		m_Velocity = m_DashSpeed * m_Direction;

}


