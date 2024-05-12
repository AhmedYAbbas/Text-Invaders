#pragma once

#include <vector>

#include "Size.h"
#include "AlienSwarm.h"
#include "Shield.h"

class Game;
class Player;
class Missile
{
public:
	Missile();
	
	void Reset();
	void Update(Player& player, Game& game);

	void SetXPosition(int xPos) { m_Position.X = xPos; }
	void SetYPosition(int yPos) { m_Position.Y = yPos; }
	
	inline const Position& GetPosition() const { return m_Position; }
	inline char GetSprite() const { return m_Sprite; }
	bool Collides(const Position& projectile, const Position& spritePosition, const Size& spriteSize);

private:
	int Collides(const std::vector<Shield> shields, Position& shieldCollisionPoint);
	bool Collides(const AlienSwarm& aliens, Position& alienCollisionPoint);

	void ResolveShieldCollision(int index, std::vector<Shield>& shields, Position& shieldCollisionPoint);
	int ResolveAlienCollision(AlienSwarm& aliens, Position& alienCollisionPoint);

private:
	char m_Sprite = '|';
	Position m_Position;
	AlienSwarm m_Aliens;
};