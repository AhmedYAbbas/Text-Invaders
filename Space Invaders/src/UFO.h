#pragma once

#include <ctime>

#include "Size.h"

class Player;
class UFO
{
public:
	UFO();
	
	void Render();
	void Reset();
	void Update(std::clock_t dt, Player& player);

private:
	void Spawn();

private:
	Position m_Position;
	Size m_SpriteSize;
	int m_Points;
};