#include "CursesUtils.h"

#include <string>

void CursesUtils::Init(bool noDelay)
{
	initscr();
	noecho();
	curs_set(false);
	nodelay(stdscr, noDelay);
	keypad(stdscr, true);
}

void CursesUtils::Shutdown()
{
	endwin();
}

void CursesUtils::Clear()
{
	clear();
}

void CursesUtils::Refresh()
{
	refresh();
}

int CursesUtils::GetScreenWidth()
{
	return COLS;
}

int CursesUtils::GetScreenHeight()
{
	return LINES;
}

int CursesUtils::GetChar()
{
	return getch();
}

void CursesUtils::DrawChar(int xPos, int yPos, char ch)
{
	mvaddch(yPos, xPos, ch);
}

void CursesUtils::DrawString(int xPos, int yPos, const std::string& str)
{
	mvprintw(yPos, xPos, str.c_str());
}

void CursesUtils::DrawSprite(int xPos, int yPos, const std::vector<std::string>& sprite, int spriteHeight, int offset)
{
	for (int h = 0; h < spriteHeight; ++h)
	{
		mvprintw(yPos + h, xPos, sprite[h + offset].c_str());
	}
}

void CursesUtils::MoveCursor(int xPos, int yPos)
{
	move(yPos, xPos);
}
