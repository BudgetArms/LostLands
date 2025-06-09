#pragma once
#include "utils.h"


class WinDoor final
{
public:
    WinDoor(const Rectf& area, bool needsAllEnemiesKilled);
    ~WinDoor() = default;

    WinDoor(const WinDoor& other) = delete;
    WinDoor(const WinDoor&& other) = delete;
    WinDoor& operator=(const WinDoor& other) = delete;
    WinDoor& operator=(const WinDoor&& other) = delete;


    void Draw() const;
    void Update();

    const Rectf& GetArea() const { return m_WinDoor; };

    bool IsActive() const { return m_bIsActive; };
    void SetActive(bool isActive) { m_bIsActive = isActive; };


private:
    Rectf m_WinDoor;
    bool m_bNeedsAllEnemiesKilled;
    bool m_bIsActive;


};


