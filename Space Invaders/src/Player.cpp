#include "Player.h"
#include "Game.h"
#include "Utility/CursesUtils.h"

Player::Player()
	: m_SpriteSize({5, 2}), m_Lives(3), m_Score(0)
{
	Reset();
}

void Player::Reset()
{
	m_Position.X = Game::GetWindowSize().Width / 2 - m_SpriteSize.Width / 2;
	m_Position.Y = Game::GetWindowSize().Height - m_SpriteSize.Height - 1;
	m_Animation = 0;
	m_Lives = 3;
	m_Missile.Reset();
}

void Player::Move(int dx)
{
	if (m_Position.X + m_SpriteSize.Width + dx > Game::GetWindowSize().Width)
		m_Position.X = Game::GetWindowSize().Width - m_SpriteSize.Width;
	else if (m_Position.X + dx < 0)
		m_Position.X = 0;
	else
		m_Position.X += dx;
}

void Player::Shoot()
{
	if (m_Missile.GetPosition().X == -1 || m_Missile.GetPosition().Y == -1)
	{
		m_Missile.SetYPosition(m_Position.Y - 1);
		m_Missile.SetXPosition(m_Position.X + m_SpriteSize.Width / 2);
	}
}

void Player::Update(std::clock_t dt, Game& game)
{
	if (Game::GetState() == GameState::Play)
		m_Missile.Update(*this, game);
	else if (Game::GetState() == GameState::PlayerDead)
		m_Animation = (m_Animation + 1) % 2;
}

void Player::Render(const std::vector<std::string>& sprite)
{
	CursesUtils::DrawSprite(m_Position.X, m_Position.Y, sprite, m_SpriteSize.Height, m_Animation * m_SpriteSize.Height);
	if (m_Missile.GetPosition().X != -1)
		CursesUtils::DrawChar(m_Missile.GetPosition().X, m_Missile.GetPosition().Y, m_Missile.GetSprite());
	mvprintw(0, 0, "Score: %i, Lives: %i", m_Score, m_Lives);
}
