# Milestone 1 - Pre-production - Game idea and Environment
## Game idea
### Summary
3D roguelike in a non-realistic medieval setting where you combat enemies on different levels.
### Gameplay
They player combats enemies using magical spells in order to progress through different floors. The player starts at floor 1, once there are no enemies left on the current floor they player advances to the next floor. The goal is to get to far as possible, the higher floor number the higher score. Each floor and its enemies gets progressively more difficult and the player gets additional upgrades.
### Screens
* Title
	* Options
	* Score/Leaderboards
* Game
### Controls
Player controls game character using mouse and keyboard input.
### Mechanics
#### Player
* Player health, player has health and if depleted, triggers end of game.

* Player combat, player has a spell which allows firing projectiles in order to damage and defeat enemies.

* Player scaling, player scales progressively with the level of the current floor. At the end of each floor, the player gets to choose between a small set of random upgrades to add to the players spell.
#### Enemy
* Enemy health, enemies has health and if depleated, enemy gets removed and counts as defeated.

* Enemy scaling, enemies scales progressively with the level of the current floor, obtaining more health and damage.

* Enemy with weak spots, enemies that can only be damaged at a certain weak spot to be defeated.
## Development environment
### Libraries
bgfx (3D), SoLoud (audio), Bullet (physics)
## Engine license
MIT License, Zlib License
## Structure
This structure is taken from the book "Game Engine Architecture" (Second Edition) in order to get a better idea of what parts could be needed. We coloured each componenet depending on if we think it is appropiate in our engine, following the rules:
* **Green** - most likely needed
* **Yellow** - might be needed
* **Red** - not needed
* **Grey** - unknown

![Group B engine structure](Group-B-structure.png)
## Code style
C++ Core Guidelines (https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md)
Google C++ Style Guide (https://google.github.io/styleguide/cppguide.html)
## Build tools
Microsoft Visual Studio 2019 Community MSVC 14.25.28610
Premake v5.0.0-alpha14
Premake
