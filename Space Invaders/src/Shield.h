#pragma once

#include <string>
#include <vector>

#include "Size.h"

class Shield
{
public:
	Shield();

	void Render();

	void SetPosition(int xPos, int yPos);
	void SetSprite(const std::vector<std::string>& sprite);
	void SetSprite(int row, int col, char letter);

	inline Position GetPosition() const { return m_Position; }
	inline char GetShieldLetter(int row, int col) const { return m_Sprite[row][col]; }
	inline const std::vector<std::string>& GetSprite() const { return m_Sprite; }

private:
	Position m_Position;
	std::vector<std::string> m_Sprite;
};