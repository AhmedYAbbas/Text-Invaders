#include <string>

#include "Utility/CursesUtils.h"
#include "Game.h"

int main()
{
	CursesUtils::Init(true);

	Game game;
	game.Update();

	CursesUtils::Shutdown();
}