#pragma once

#include <curses.h>
#include <string>
#include <vector>

enum ArrowKeys
{
	Up = KEY_UP,
	Down = KEY_DOWN,
	Left = KEY_LEFT,
	Right = KEY_RIGHT
};

class CursesUtils
{
public:
	static void Init(bool noDelay);
	static void Shutdown();
	static void Clear();
	static void Refresh();
	static int GetScreenWidth();
	static int GetScreenHeight();
	static int GetChar();
	static void DrawChar(int xPos, int yPos, char ch);
	static void DrawString(int xPos, int yPos, const std::string& str);
	static void DrawSprite(int xPos, int yPos, const std::vector<std::string>& sprite, int spriteHeight, int offset = 0);
	static void MoveCursor(int xPos, int yPos);
};