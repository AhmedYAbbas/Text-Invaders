# Text Invaders

A classic text-based space invaders game written entirely in C++ using PDCurses.

[![Watch the video](https://img.youtube.com/vi/FG-AP8WulY4/hqdefault.jpg)](https://www.youtube.com/embed/FG-AP8WulY4)

***

## Building the Project
The project only supports x86 Windows for now!
1. Clone the repository.
2. Open `Space Invaders.sln` and make sure the platform is `x86` then hit `F5`.
3. An error will appear that `pdcurses.dll` is missing.
4. Extract `pdcurses.zip` and copy `pdcurses.dll` from the extracted folder.
5. Navigate to the `bin/Win32/Your-Configuration/` folder and paste the dll.
6. Open `Space Invaders.sln` and hit `F5` again.

***

## Features

1. Aliens in different rows have different scores.
2. Mystery UFO.
3. Destructable shields.
4. Aliens move sideways and when they hit the sides they move one row down.
5. Aliens move faster when destroyed or each time they move downwards.
6. Each level, aliens start one row lower than the previous level.
7. If the aliens reach the bottom of the screen, the game ends.
8. Aliens drop bombs (max 3 bombs), and the player could only 1 missile at a time (they do not collide).
9. Player and aliens animations.
10. Intro, gameplay, gameover, and highscore screens.
11. Highscore leaderboard. (save/load system).
12. etc...
