# Dungeon Crawler

A top-down dungeon crawler written in C++ with SFML. Explore a five-room dungeon, fight your way through it, and defeat the boss.


## Objective

Clear the **Armory**, the **Library** and the **Storage** room, then defeat the boss waiting in the **Boss Room**.

The boss door (the south door of the Entrance) is **red and sealed** until all three of those rooms are cleared. Once they are, it turns brown and opens.

```text
              [ Armory ]
                  |
[ Library ] - [ Entrance ] - [ Storage ]
                  |
             [ Boss Room ]
```

## Controls

| Key | Action |
|---|---|
| `W` `A` `S` `D` | Move |
| `Space` | Attack (hits every enemy in range) |
| `Esc` | Restart after winning or dying |

Walk into a brown door to move to the next room.

## How the game works

- **Leveling.** Each goblin gives 5 XP, and 10 XP is one level. A level-up fully heals you and gives +5 max health, +1 strength and +1 defense.
- **Damage.** `damage = max(1, attacker strength - defender defense)`. Every hit does at least 1.
- **Enemies** chase you from the moment you enter their room, and they stay dead once defeated.
- **The boss** is only realistically beatable once you have cleared all three rooms, which brings you to level 4.
- **Winning or dying** freezes the game and shows an overlay. Press `Esc` to start over with a fresh dungeon.

## Running the game

### Requirements

- 64-bit Windows
- The [Microsoft Visual C++ Redistributable (x64)](https://aka.ms/vs/17/release/vc_redist.x64.exe), only if the game fails to start with a missing `VCRUNTIME140` or `MSVCP140` error

### Steps

1. Unzip the release folder anywhere.
2. Run `Dungeons-CAPSTONE.exe` **from inside that folder**.

The folder must contain:

```text
Dungeons-CAPSTONE.exe
sfml-graphics-3.dll
sfml-window-3.dll
sfml-system-3.dll
assets/
    font.ttf
    LICENSE-font.txt
```

> The game loads `assets/font.ttf` relative to its working directory. Launching it through a shortcut with a different "Start in" folder, or from another directory, will make it fail to start.

### Troubleshooting

| Problem | Cause and fix |
|---|---|
| "The code execution cannot proceed because `sfml-*.dll` was not found" | An SFML DLL is missing from the exe's folder. Copy all three into it |
| `VCRUNTIME140.dll` or `MSVCP140.dll` not found | Install the Visual C++ Redistributable linked above |
| Window closes instantly, or no window appears | Run the exe from PowerShell to read the error. `Fatal error: Failed to load assets/font.ttf` means the `assets` folder is missing or you launched from the wrong directory |
| Text is missing but the game runs | The font failed to load. Check `assets/font.ttf` exists beside the exe |

## Building from source

### Requirements

- Visual Studio 18.11.0 2026 with the **Desktop development with C++** workload
- SFML 3.1.0, 64-bit, built for Visual C++ 
- A C++17 compiler (SFML 3 and `std::filesystem` both require it)

### Steps

1. Clone the repository.
2. Download SFML and extract it somewhere, for example `C:\SFML`.
3. Open the `.sln` file in Visual Studio.
4. Select **Release | x64**.
5. In the project properties (**All Configurations** unless noted), check these settings:

   | Setting | Value |
   |---|---|
   | C/C++ → General → Additional Include Directories | `<SFML>\include` |
   | Linker → General → Additional Library Directories | `<SFML>\lib` |
   | Linker → Input → Additional Dependencies (Release) | `sfml-graphics.lib;sfml-window.lib;sfml-system.lib` |
   | Linker → Input → Additional Dependencies (Debug) | `sfml-graphics-d.lib;sfml-window-d.lib;sfml-system-d.lib` |
   | C/C++ → Code Generation → Runtime Library | Multi-threaded DLL (`/MD`) for Release, Multi-threaded Debug DLL (`/MDd`) for Debug |
   | C/C++ → Language → C++ Language Standard | ISO C++17 or later |

6. Build the solution.
7. Copy the three SFML DLLs from `<SFML>\bin` and the project's `assets` folder next to the built exe. Use the `-d` DLLs (`sfml-graphics-d-3.dll` and so on) for Debug builds, and the plain ones for Release. A post-build event can do this for you:

   ```text
   xcopy /y /e /i "$(ProjectDir)assets" "$(OutDir)assets"
   xcopy /y "<SFML>\bin\sfml-graphics-3.dll" "$(OutDir)"
   xcopy /y "<SFML>\bin\sfml-window-3.dll" "$(OutDir)"
   xcopy /y "<SFML>\bin\sfml-system-3.dll" "$(OutDir)"
   ```

When running from Visual Studio, the working directory is the project folder, so `assets/font.ttf` must exist there as well.

## Project structure

| File | Responsibility |
|---|---|
| `main.cpp` | Entry point. Creates the game and reports startup errors |
| `game.hpp` / `game.cpp` | Game loop, input, movement, combat resolution, rendering, HUD and win/lose states |
| `character.hpp` / `character.cpp` | The player: stats, leveling, attack cooldown |
| `enemies.hpp` / `enemies.cpp` | Enemy stats, chasing, attack cooldown and the boss flag |
| `room.hpp` / `room.cpp` | A room's tile layout, neighbors and enemies, plus `isCleared()` |
| `map.hpp` / `map.cpp` | Builds the five rooms, links them, places enemies and answers `isBossUnlocked()` |
| `assets/` | Font and its license |

## Known issues

- Enemies chase from the moment you enter a room (there is no aggro range), and they can overlap each other.
- Knockback is skipped entirely if it would push an enemy into a wall.
- Your attack range (50 px) is longer than an enemy's (40 px), so a careful player can hit enemies from just outside their reach.
- The game must be launched from its own folder (see above).
- There is no pause, menu, sound or save system.

## Future work

- Inventory, loot and health pickups. The related functions in `Character` are empty stubs.
- Replace the string-based door directions with an `enum class Direction`.
- Split `Game::update()` into player, enemy and attack functions.
- Per-axis wall sliding for movement and knockback.
- Enemy separation and an aggro range.

## Credits and licenses

- Built with [SFML](https://www.sfml-dev.org/), licensed under the zlib/libpng license.
- Font: Open Sans, licensed under SIL Open Font License. The license text is in `assets/OFL.txt`.
- Created by Landon Shoue for Senior Seminar in Mathematics.
