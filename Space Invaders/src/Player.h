#pragma once

#include <vector>
#include <ctime>
#include <string>

#include "Size.h"
#include "Missile.h"

class Game;
class Player
{
public:
	Player();

	void Reset();

	void Move(int dx);
	void Shoot();
	void Update(std::clock_t dt, Game& game);
	void Render(const std::vector<std::string>& sprite);

	inline void SetPlayerName(int index, char letter) { m_Name[index] = letter; }
	inline void SetAnimation(int index) { m_Animation = index; }
	inline void DecreaseLives() { --m_Lives; }
	inline void AddScore(int amount) { m_Score += amount; }

	inline int GetLives() const { return m_Lives; }
	inline int GetScore() const { return m_Score; }
	inline std::string GetName() const { return std::string(m_Name); }
	inline const Position& GetPosition() const { return m_Position; }
	inline const Size& GetSpriteSize() const { return m_SpriteSize; }
	inline const Missile& GetMissile() const { return m_Missile; }
	inline Missile& GetMissile() { return m_Missile; }

private:
	char m_Name[4];
	Position m_Position;
	Missile m_Missile;
	Size m_SpriteSize;
	int m_Animation;
	int m_Lives;
	int m_Score;
};