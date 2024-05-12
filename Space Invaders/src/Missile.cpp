#include "Missile.h"
#include "Player.h"
#include "Game.h"

static const int NUM_ALIEN_ROWS = 5;
static const int NUM_ALIEN_COLUMNS = 11;
static const int ALIENS_X_PADDING = 1;
static const int ALIENS_Y_PADDING = 1;

const int SHIELD_SPRITE_WIDTH = 7;
const int SHIELD_SPRITE_HEIGHT = 3;

Missile::Missile()
	: m_Position({-1, -1})
{
}

void Missile::Reset()
{
	m_Position.X = -1;
	m_Position.Y = -1;
}

void Missile::Update(Player& player, Game& game)
{
	if (m_Position.Y != -1)
	{
		m_Position.Y -= 1;
		if (m_Position.Y < 0)
			Reset();

		Position shieldCollisionPoint;
		int shieldIndex = Collides(game.GetShields(), shieldCollisionPoint);
		if (shieldIndex != -1)
		{
			Reset();
			ResolveShieldCollision(shieldIndex, game.GetShields(), shieldCollisionPoint);
		}

		Position alienCollisionPoint;
		if (Collides(game.GetAlienSwarm(), alienCollisionPoint))
		{
			Reset();
			player.AddScore(ResolveAlienCollision(game.GetAlienSwarm(), alienCollisionPoint));
		}
	}
}

int Missile::Collides(const std::vector<Shield> shields, Position& shieldCollisionPoint)
{
	shieldCollisionPoint.X = -1;
	shieldCollisionPoint.Y = -1;

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

bool Missile::Collides(const AlienSwarm& aliens, Position& alienCollisionPoint)
{
	alienCollisionPoint.X = -1;
	alienCollisionPoint.Y = -1;

	for (int row = 0; row < NUM_ALIEN_ROWS; ++row)
	{
		for (int col = 0; col < NUM_ALIEN_COLUMNS; ++col)
		{
			int xPos = aliens.GetPosition().X + col * (aliens.GetSpriteSize().Width + ALIENS_X_PADDING);
			int yPos = aliens.GetPosition().Y + row * (aliens.GetSpriteSize().Height + ALIENS_Y_PADDING);

			if (aliens.GetAlien(row, col) == AlienState::Alive &&
				m_Position.X >= xPos && m_Position.X < xPos + aliens.GetSpriteSize().Width &&
				m_Position.Y >= yPos && m_Position.Y < yPos + aliens.GetSpriteSize().Height)
			{
				alienCollisionPoint.X = col;
				alienCollisionPoint.Y = row;
				return true;
			}
		}
	}
	return false;
}

bool Missile::Collides(const Position& projectile, const Position& spritePosition, const Size& spriteSize)
{
	return (projectile.X >= spritePosition.X && projectile.X < (spritePosition.X + spriteSize.Width) &&
		projectile.Y >= spritePosition.Y && projectile.Y < (spritePosition.Y + spriteSize.Height));
}

void Missile::ResolveShieldCollision(int index, std::vector<Shield>& shields, Position& shieldCollisionPoint)
{
	shields[index].SetSprite(shieldCollisionPoint.Y, shieldCollisionPoint.X, ' ');
}

int Missile::ResolveAlienCollision(AlienSwarm& aliens, Position& alienCollisionPoint)
{
	aliens.SetAlienState(alienCollisionPoint.Y, alienCollisionPoint.X, AlienState::Exploding);
	aliens.DecreaseNumOfAliens();

	if (aliens.GetExplosionTimer() == -1)
		aliens.SetExplosionTimer(4);

	if (alienCollisionPoint.Y == 0)
		return 30;
	else if (alienCollisionPoint.Y >= 1 && alienCollisionPoint.Y < 3)
		return 20;
	else
		return 10;
}
