#pragma once

#include <string>
#include <vector>

#include "Size.h"

class Shield;
class AlienSwarm;
class AlienBomb
{
public:
	AlienBomb();

	inline void SetXPosition(int xPos) { m_Position.X = xPos; }
	inline void SetYPosition(int yPos) { m_Position.Y = yPos; }
	inline void Move(int amount) { m_Position.Y += amount; }
	inline void SetAnimation(int anim) { m_Animation = anim; }

	inline const std::string& GetSprite() const { return m_Sprite; }
	inline const Position& GetPosition() const { return m_Position; }
	inline int GetAnimation() const { return m_Animation; }

	int Collides(const std::vector<Shield> shields, Position& shieldCollisionPoint);
	bool Collides(const Position& projectile, const Position& spritePosition, const Size& spriteSize);
	void ResolveShieldCollision(int index, std::vector<Shield>& shields, Position& shieldCollisionPoint);

private:
	std::string m_Sprite;
	Position m_Position;
	int m_Animation;
};