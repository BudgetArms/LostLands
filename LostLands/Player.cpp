#include "pch.h"
#include "Player.h"

#include "InputManager.h"
#include "EntityManager.h"
#include "Bullet.h"


Player::Player(const Point2f& position) :
    Character(position)
{
    std::cout << "Created Player" << "\n";

    m_Color = Color4f(1.f, 1.f, 1.f, 1.f);
    m_HealthBarColor = Color4f(0.f, 1.f, 0.f, 0.8f);
    m_Width = 20;
    m_Height = 20;
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

Player::~Player()
{
    std::cout << "Destroyed Player" << "\n";
}



void Player::Draw() const
{
    // draw Player
    utils::SetColor(m_Color);
    utils::FillEllipse(m_Position, m_Width / 2, m_Height / 2);

    // draw HealthBar
    constexpr float healthBarOffset{ 25.f };
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

}

void Player::Update(float elapsedSec)
{
    Character::Update(elapsedSec);

    if (m_IFramesCountDown >= 0.0f)
        m_IFramesCountDown -= elapsedSec;

    HandleKeyboardInput(elapsedSec);
    HandleMouseInput(elapsedSec);

    HandleWallCollisions();


    m_AccumulatedSecBullets += elapsedSec;

    if (m_AccumulatedSecBullets >= 1 / m_BulletsPerSecond)
    {
        m_AccumulatedSecBullets = 0.f;

        m_bCanShoot = true;

    }

    m_MirrorTimer += elapsedSec;
    if (m_MirrorTimer >= m_MirrorCooldown)
    {
        m_MirrorTimer = 0.f;
        m_bCanMirror = true;
    }


}

Rectf& Player::GetHitBox()
{
    m_HitBox.left = m_Position.x - m_Width / 2;
    m_HitBox.bottom = m_Position.y - m_Height / 2;

    return m_HitBox;
}

void Player::Damage(float damage)
{
    if (m_IFramesCountDown <= 0)
        Character::Damage(damage);
}

void Player::Shoot()
{
    if (!m_bCanShoot || !m_bIsShootingEnabled)
        return;

    //std::cout << "Player: Shoot\n";

    int x{}, y{};
    SDL_GetMouseState(&x, &y);


    y = -y + int(g_Window.height);
    const float directionAngle{ utils::g_toDegrees * atan2f(y - m_Position.y, x - m_Position.x) };

    EntityManager::GetInstance().SpawnBullet(*this, m_Position, directionAngle);
    m_bCanShoot = false;

}

void Player::Mirror()
{
    if (!m_bCanMirror || !m_bIsMirroringEnabled)
        return;

    std::cout << "Mirror\n";
    m_Position = Point2f(g_Window.width - m_Position.x, g_Window.height - m_Position.y);

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
            //std::cout << "moving\n";
            m_Velocity.x += m_Input.x * m_Acceleration * elapsedSec;
            m_Velocity.y += m_Input.y * m_Acceleration * elapsedSec;

            if (m_Velocity.Norm() > m_Speed)
                m_Velocity = m_Speed * m_Velocity.Normalized();

            m_Direction = m_Velocity.Normalized();
        }
        else
        {
            Friction(elapsedSec);
        }

    }

    if (m_Velocity.Norm() != 0)
    {
        //std::cout << "Velocity, X: " << m_Velocity.x << ", Y: " << m_Velocity.y << '\n';
        //std::cout << "Velocity: " << m_Velocity.Norm() << '\n';
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

    //std::cout << "Friction\n";
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
    //std::cout << "Dashing\n";
    m_bIsDashing = true;
    m_DashTimer = m_DashTime + m_DashCooldown;

    // use input, if there is input
    if (m_Input.Norm() != 0)
    {
        // if there is input
        //std::cout << "Dash: Using Input\n";
        m_Velocity = m_DashSpeed * m_Input;
    }
    else if (m_Velocity.Norm() == 0)
    {
        // if not moving, use the direciton
        //std::cout << "Dash: Using Direction\n";
        m_Velocity = m_DashSpeed * m_Direction;
    }


}

void Player::HandleWallCollisions()
{
    // hit left wall
    if (m_Position.x - m_Width / 2 <= g_SmallWindow.left)
    {
        m_Position.x = g_SmallWindow.left + m_Width / 2;
        if (m_bIsDashing)
            m_Velocity.x *= -1;
        else
            m_Velocity.x *= -m_BouncinessWalls;
    }

    // hit right wall
    if (m_Position.x + m_Width / 2 >= g_SmallWindow.left + g_SmallWindow.width)
    {
        m_Position.x = g_SmallWindow.left + g_SmallWindow.width - m_Width / 2;
        if (m_bIsDashing)
            m_Velocity.x *= -1;
        else
            m_Velocity.x *= -m_BouncinessWalls;
    }

    // hit bottom wall
    if (m_Position.y - m_Height / 2 <= g_SmallWindow.bottom)
    {
        m_Position.y = g_SmallWindow.bottom + m_Height / 2;
        if (m_bIsDashing)
            m_Velocity.y *= -1;
        else
            m_Velocity.y *= -m_BouncinessWalls;
    }

    // hit top wall
    if (m_Position.y + m_Height / 2 >= g_SmallWindow.bottom + g_SmallWindow.height)
    {
        m_Position.y = g_SmallWindow.bottom + g_SmallWindow.height - m_Height / 2;
        if (m_bIsDashing)
            m_Velocity.y *= -1;
        else
            m_Velocity.y *= -m_BouncinessWalls;
    }

}

