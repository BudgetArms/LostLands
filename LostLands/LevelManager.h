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
	const std::string& GetResourcePath() const { return m_ResourcePath; };

	void Level4DrawThings() const;


	std::unique_ptr<Texture> m_uTextInfo;
	CheckPoint* m_CurrentCheckPoint{ nullptr };
	Point2f m_CurrentSpawnPosition{};


private:
	friend class Singleton<LevelManager>;
	LevelManager();
	~LevelManager(); // bc of class forward


	const std::string m_ResourcePath{ "" };

	std::unique_ptr<Texture>		m_uTextWon;
	std::unique_ptr<Texture>		m_uTextDead;

	std::unique_ptr<Texture>		m_uTextLevel4;
	std::unique_ptr<Texture>		m_uTextMirror;
	std::unique_ptr<Texture>		m_uTextDash;

	std::vector<std::function<void()>> m_ArrLoadLevel{};


	int m_CurrentLevel{ 0 };
	int m_NrOfLevels{ 5 };


	bool m_bWonGame{ false };
	bool m_bLostGame{ false };


	// lvl 4 specific
	bool m_bHasDiscoveredMiddleRight{ false };
	bool m_bHasDiscoveredMiddleCenter{ false };
	bool m_bHasDiscoveredMiddleMirror{ false };
	bool m_bHasDiscoveredMiddleLeft{ false };
	bool m_bHasDiscoveredBottom{ false };


};


