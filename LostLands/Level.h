#pragma once



class Level final
{
public:
    Level();

    Level(const Level& other) = delete;
    Level(Level&& other) = delete;
    Level& operator=(const Level& other) = delete;
    Level& operator=(Level&& other) = delete;

    ~Level();

    void AddPlayer(const Point2f& position);
    void AddEnemy(const Point2f& position);
    void SetWinDoor(const Rectf& area);

    bool IsPlayerDead() const { return false; }
    bool HasPlayerFinished() const { return false; }


private:

    bool m_bLevelWon{ false };
    bool m_bAllEnemiesDead{ false };


protected:


};


