#pragma once

#include <functional>
#include <memory>

#include "Singleton.h"


class Texture;
class CheckPoint;

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

	void Level2DrawThings() const;
	void Level3DrawThings() const;
	void Level4DrawThings() const;

	std::unique_ptr<Texture>& GetTextInfo() { return m_uTextInfo; };

	const std::string m_FontPath{ "Aovel.ttf" };

	CheckPoint* m_CurrentCheckPoint{ nullptr };
	Point2f m_CurrentSpawnPosition{};


private:
	friend class Singleton<LevelManager>;
	LevelManager();
	~LevelManager(); // bc of class forward

	// needs to be before unique_ptr's bc initializing order
	const int m_FontSizeSmall{ 26 };
	const int m_FontSizeMedium{ 30 };
	const int m_FontSizeBig{ 50 };

	std::unique_ptr<Texture> m_uTextInfo;
	std::unique_ptr<Texture> m_uTextWon;
	std::unique_ptr<Texture> m_uTextDead;
	std::unique_ptr<Texture> m_uTextLevelName;

	std::unique_ptr<Texture> m_uTextLevel4;
	std::unique_ptr<Texture> m_uTextLevel5;
	std::unique_ptr<Texture> m_uTextMirror;
	std::unique_ptr<Texture> m_uTextDash;

	std::vector<std::function<void()>> m_ArrLoadLevel{};


	const Color4f m_HiddenColor{ Color4f(0,0,0.2f, 1.f) };

	int m_CurrentLevel{ 0 };
	int m_NrOfLevels;


	bool m_bWonGame{ false };
	bool m_bLostGame{ false };


	// lvl 2 specific
	bool m_bHasLvl2DiscoveredRightBottom;
	bool m_bHasLvl2DiscoveredRightTop;
	bool m_bHasLvl2DiscoveredMiddleLeft;
	bool m_bHasLvl2DiscoveredMiddleRight;


	// lvl 3 specific
	bool m_bHasDiscoveredMiddleRight;
	bool m_bHasDiscoveredMiddleCenter;
	bool m_bHasDiscoveredMiddleMirror;
	bool m_bHasDiscoveredMiddleLeft;
	bool m_bHasDiscoveredBottom;

	// lvl 4 specific
	bool m_bHasDiscovered1;
	bool m_bHasDiscovered2;
	bool m_bHasDiscovered3;
	bool m_bHasDiscovered4;
	bool m_bHasDiscovered5;
	bool m_bHasDiscovered6;


};


