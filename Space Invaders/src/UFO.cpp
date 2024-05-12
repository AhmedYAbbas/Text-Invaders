#include <random>

#include "Player.h"
#include "Missile.h"
#include "Game.h"
#include "UFO.h"
#include "Utility/CursesUtils.h"

static const int ALIEN_SPRITE_HEIGHT = 2;
static const std::vector<std::string> ALIEN_UFO_SPRITE = {"_/oo\\_", "=q==p="};

UFO::UFO()
{
	Reset();
}

void UFO::Render()
{
	if (m_Position.X != -1)
		CursesUtils::DrawSprite(m_Position.X, m_Position.Y, ALIEN_UFO_SPRITE, ALIEN_SPRITE_HEIGHT);
}

void UFO::Reset()
{
	m_SpriteSize.Width = 6;
	m_SpriteSize.Height = 2;
	m_Points = ((std::rand() % 4) + 1) * 50;
	m_Position.X = -1;
	m_Position.Y = m_SpriteSize.Height;
}

void UFO::Update(std::clock_t dt, Player& player)
{
	if (Game::GetState() == GameState::Play)
	{
		if (m_Position.X == -1)
		{
			if (Game::GetGameTimer() % 500 == 13)
				Spawn();
		}
		else
		{
			if (player.GetMissile().Collides(player.GetMissile().GetPosition(), m_Position, m_SpriteSize))
			{
				player.AddScore(m_Points);
				player.GetMissile().Reset();
				Reset();
			}
			else
			{
				m_Position.X += 1;
				if (m_Position.X + m_SpriteSize.Width >= Game::GetWindowSize().Width)
					Reset();
			}
		}
	}
}

void UFO::Spawn()
{
	m_Position.X = 0;
}
