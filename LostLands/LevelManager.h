#pragma once

#include <functional>
#include "Level.h"

#include "Singleton.h"


class Texture;
class Level;

class LevelManager final : public Singleton<LevelManager>
{
public:

    void Start();

    void Draw() const;
    void Update(float elapsedSec);



    void SetLevel(int level);
    void ResetLevel();

    bool HasPlayerFinishedLevel() const;

    Rectf m_WinDoor{};

private:
    friend class Singleton<LevelManager>;
    LevelManager();

    const std::string m_ResourcePath;

    std::unique_ptr<Texture>		m_uTextInfo;
    std::unique_ptr<Texture>		m_uTextWon;
    std::unique_ptr<Texture>		m_uTextDead;


    std::vector<std::function<void()>> m_ArrLoadLevel{};


    int m_NrCurrentLevel{ 0 };
    int m_NrOfLevels{ 5 };

    bool m_bWonGame{ false };
    bool m_bLostGame{ false };


};


