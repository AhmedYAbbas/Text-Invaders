#include "Shield.h"
#include "Game.h"
#include "Utility/CursesUtils.h"

static const int SHIELD_SPRITE_HEIGHT = 3;

Shield::Shield()
	: m_Position({0, 0}), m_Sprite({"/IIIII\\", "IIIIIII", "I/   \\I"})
{
}

void Shield::Render()
{
	CursesUtils::DrawSprite(m_Position.X, m_Position.Y, m_Sprite, SHIELD_SPRITE_HEIGHT);
}

void Shield::SetPosition(int xPos, int yPos)
{
	m_Position = {xPos, yPos};
}

void Shield::SetSprite(const std::vector<std::string>& sprite)
{
	m_Sprite = sprite;
}

void Shield::SetSprite(int row, int col, char letter)
{
	m_Sprite[row][col] = letter;
}
