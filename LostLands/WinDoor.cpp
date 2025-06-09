#include "pch.h"
#include "WinDoor.h"

//#include "EntityManager.h"


WinDoor::WinDoor(const Rectf& area, bool needsAllEnemiesKilled) :
    m_WinDoor{ area },
    m_bNeedsAllEnemiesKilled{ needsAllEnemiesKilled },
    m_bIsActive{ !needsAllEnemiesKilled }
{
}

void WinDoor::Draw() const
{
    if (!m_bIsActive)
        return;

    utils::SetColor(1, 1, 0, 0.4f);
    utils::FillRect(m_WinDoor);
}

void WinDoor::Update()
{
    if (!m_bNeedsAllEnemiesKilled)
        return;

    if (true)
        //if (EntityManager::GetInstance().GetEnemies().empty())
        m_bIsActive = true;
    else
        m_bIsActive = false;

}


