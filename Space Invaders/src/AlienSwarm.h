#pragma once

#include <ctime>
#include <vector>

#include "Size.h"
#include "AlienBomb.h"

enum class AlienState
{
	Alive = 0,
	Dead,
	Exploding
};

class Player;
class Shield;
class Game;
class AlienSwarm
{
public:
	AlienSwarm();

	void Render();

	void Reset();
	void ResetMovementTimer();
	void Update(std::clock_t dt, Game& game, const Player& player);
	bool UpdateBombs(std::vector<Shield>& shields, const Player& player);

	inline void SetAlienState(int row, int col, AlienState state) { m_Aliens[row][col] = state; }
	inline void DecreaseNumOfAliens() { --m_NumOfAliensLeft; }
	inline void SetExplosionTimer(int value) { m_ExplosionTimer = value; }

	inline Position GetPosition() const { return m_Position; }
	inline Size GetSpriteSize() const { return m_SpriteSize; }
	inline int GetExplosionTimer() const { return m_ExplosionTimer; }
	inline const AlienState& GetAlien(int row, int col) const { return m_Aliens[row][col]; }

private:
	void FindEmptyRowsAndColumns(int& emptyColsLeft, int& emptyColsRight, int& emptyRowsBottom);
	void DestroyShields(std::vector<Shield>& shields);
	void CollideShieldsWithAlien(std::vector<Shield>& shields, int alienPositionX, int alienPositionY, const Size& spriteSize);
	bool ShouldShootBomb();
	void ShootBomb(int columnToShoot);

private:
	Position m_Position;
	AlienState m_Aliens[5][11];
	AlienBomb m_Bombs[3];
	Size m_SpriteSize;
	int m_Animation;
	int m_Direction;
	int m_NumberOfBombsInPlay;
	int m_MovementTimer;
	int m_ExplosionTimer;
	int m_NumOfAliensLeft;
	int m_Line;
};