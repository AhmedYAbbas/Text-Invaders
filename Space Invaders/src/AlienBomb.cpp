#include "AlienBomb.h"
#include "AlienSwarm.h"
#include "Shield.h"

AlienBomb::AlienBomb()
	: m_Sprite("\\|/-"), m_Position({-1, -1}), m_Animation(0)
{
}

int AlienBomb::Collides(const std::vector<Shield> shields, Position& shieldCollisionPoint)
{
	shieldCollisionPoint.X = -1;
	shieldCollisionPoint.Y = -1;

	const int SHIELD_SPRITE_WIDTH = 7;
	const int SHIELD_SPRITE_HEIGHT = 3;

	if (m_Position.Y != -1)
	{
		for (size_t i = 0; i < shields.size(); ++i)
		{
			if (m_Position.X >= shields[i].GetPosition().X && m_Position.X < (shields[i].GetPosition().X + SHIELD_SPRITE_WIDTH)
				&& (m_Position.Y >= shields[i].GetPosition().Y && m_Position.Y < shields[i].GetPosition().Y + SHIELD_SPRITE_HEIGHT)
				&& shields[i].GetShieldLetter(m_Position.Y - shields[i].GetPosition().Y, m_Position.X - shields[i].GetPosition().X) != ' ')
			{
				shieldCollisionPoint.X = m_Position.X - shields[i].GetPosition().X;
				shieldCollisionPoint.Y = m_Position.Y - shields[i].GetPosition().Y;
				return i;
			}
		}
	}
	return -1;
}

bool AlienBomb::Collides(const Position& projectile, const Position& spritePosition, const Size& spriteSize)
{
	return (projectile.X >= spritePosition.X && projectile.X < (spritePosition.X + spriteSize.Width) &&
	projectile.Y >= spritePosition.Y && projectile.Y < (spritePosition.Y + spriteSize.Height));
}

void AlienBomb::ResolveShieldCollision(int index, std::vector<Shield>& shields, Position& shieldCollisionPoint)
{
	shields[index].SetSprite(shieldCollisionPoint.Y, shieldCollisionPoint.X, ' ');
}
