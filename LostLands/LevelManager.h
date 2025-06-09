#pragma once

#include <functional>
#include <memory>

#include "Singleton.h"


class Texture;

class LevelManager final : public Singleton<LevelManager>
{
public:
    void Start();

    void Draw() const;
    void Update(float elapsedSec);

    void SkipLevel();
    void SetLevel(int level);
    void ResetLevel();

    bool HasPlayerFinishedLevel() const;
    const std::string& GetResourcePath() const { return m_ResourcePath; };


    std::unique_ptr<Texture> m_uTextInfo;


private:
    friend class Singleton<LevelManager>;
    LevelManager();
    ~LevelManager(); // bc of class forward


    const std::string m_ResourcePath{ "" };

    std::unique_ptr<Texture>		m_uTextWon;
    std::unique_ptr<Texture>		m_uTextDead;

    std::unique_ptr<Texture>		m_uTextMirror;
    std::unique_ptr<Texture>		m_uTextDash;

    std::vector<std::function<void()>> m_ArrLoadLevel{};

    int m_CurrentLevel{ 0 };
    int m_NrOfLevels{ 5 };

    bool m_bWonGame{ false };
    bool m_bLostGame{ false };


};


