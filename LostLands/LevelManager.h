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

	void Level4DrawThings() const;
	void Level5DrawThings() const;

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



	int m_CurrentLevel{ 0 };
	int m_NrOfLevels;


	bool m_bWonGame{ false };
	bool m_bLostGame{ false };


	// lvl 4 specific
	bool m_bHasDiscoveredMiddleRight{ false };
	bool m_bHasDiscoveredMiddleCenter{ false };
	bool m_bHasDiscoveredMiddleMirror{ false };
	bool m_bHasDiscoveredMiddleLeft{ false };
	bool m_bHasDiscoveredBottom{ false };

	// lvl 5 specific
	bool m_bHasDiscovered1{ false };
	bool m_bHasDiscovered2{ false };
	bool m_bHasDiscovered3{ false };
	bool m_bHasDiscovered4{ false };
	bool m_bHasDiscovered5{ false };
	bool m_bHasDiscovered6{ false };


};


