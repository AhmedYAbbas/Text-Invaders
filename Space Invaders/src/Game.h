#pragma once

#include <ctime>
#include <vector>

#include "Size.h"
#include "Player.h"
#include "AlienSwarm.h"
#include "UFO.h"

enum class GameState
{
	Intro = 0,
	HighScore,
	Play,
	PlayerDead,
	Wait,
	GameOver
};


struct Score
{
	Score() = default;

	Score(const Score& score)
	{
		Value = score.Value;
		Name = score.Name;
	}

	int Value;
	std::string Name;
};

struct HighScoreTable
{
	std::vector<Score> Scores;
};

class Shield;
class Game
{
public:
	Game();

	void Update();
	void ResetGame();

	inline void SetGameState(GameState state) { m_CurrentState = state; }
	inline void SetLevel(int level) { m_Level = level; }
	inline void SetWaitTimer(int time) { m_WaitTimer = time; }

	inline static Size GetWindowSize() { return m_WindowSize; }
	inline static int GetLevel() { return m_Level; }
	inline static GameState GetState() { return m_CurrentState; }
	inline static std::clock_t GetGameTimer() { return m_GameTimer; }

	inline const std::vector<Shield>& GetShields() const { return m_Shields; }
	inline std::vector<Shield>& GetShields() { return m_Shields; }
	inline const AlienSwarm& GetAlienSwarm() const { return m_AlienSwarm; }
	inline AlienSwarm& GetAlienSwarm() { return m_AlienSwarm; }

private:
	void Render();

	void ResetGameOverCursorPos();
	void ResetShields();

	int HandleInput();

	void AddHighScore(int score, const std::string& name);
	void LoadHighScores();
	void SaveHighScore();

	void DrawGameOverScreen();
	void DrawIntroScreen();
	void DrawHighScoreTable();

private:
	static Size m_WindowSize;
	static int m_Level;
	static GameState m_CurrentState;
	static std::clock_t m_GameTimer;

	int m_WaitTimer;

	int m_GameOverHPosCursor = 0;
	int m_GameOverVPosCursor[3];

	Player m_Player;
	std::vector<Shield> m_Shields;
	AlienSwarm m_AlienSwarm;
	UFO m_UFO;
	HighScoreTable m_HighScoreTable;
};