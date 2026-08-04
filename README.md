# Text Invaders

A Space Invaders clone that runs entirely in the terminal, written in C++ with [PDCurses](https://pdcurses.org/).

[![Watch the gameplay video](https://img.youtube.com/vi/FG-AP8WulY4/hqdefault.jpg)](https://www.youtube.com/embed/FG-AP8WulY4)

***

## Features

- Alien rows worth different point values, plus a mystery UFO for bonus points
- Destructible shields that erode as they take fire
- Aliens sweep side to side, drop a row on hitting a wall, and speed up as they're destroyed
- Each new level starts the alien formation one row lower than the last
- Game ends if the aliens reach the bottom of the screen
- Aliens drop up to 3 bombs at once; the player fires one missile at a time
- Sprite-based animations for the player and aliens
- Intro, gameplay, game over, and high score screens
- Persistent high score leaderboard (save/load to file)

***

## Controls

| Key | Action |
| --- | --- |
| `←` / `→` | Move the player / navigate menus |
| `Space` | Shoot / confirm |
| `S` | View high scores (from the intro screen) |
| `Q` | Quit |

***

## Building the Project

> **Note:** Currently Windows (x86) only, built with Visual Studio.

1. Clone the repository.
2. Open `Space Invaders.sln` in Visual Studio, making sure the platform is set to `x86`, then press `F5`.
3. The build will fail to run because `pdcurses.dll` is missing.
4. Extract `pdcurses.zip` and copy `pdcurses.dll` from the extracted folder.
5. Paste it into `bin/Win32/<Your-Configuration>/`.
6. Press `F5` again to build and run.

***

## Tech Stack

- **Language:** C++
- **Rendering:** [PDCurses](https://pdcurses.org/) (terminal/console UI)
- **Build system:** Visual Studio solution (`.sln` / `.vcxproj`)
