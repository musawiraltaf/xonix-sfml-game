# Xonix (SFML, C++17)

A modernised **Xonix-inspired arcade game** built in **C++** with **SFML** and organised around a **state-based game architecture**. The project includes single-player gameplay, local multiplayer support, a save/load system, theme customisation, leaderboard tracking, and a lightweight social layer with friends and local tournament flow.

## Project highlights

- **State-driven UI and gameplay flow**
  - Login / registration
  - Main menu
  - Single-player menu with save list
  - Multiplayer menu
  - Matchmaking room / tournament flow
  - Profile, settings, themes, leaderboard, and friends screens

- **Core gameplay systems**
  - Grid-based Xonix-style territory capture
  - Enemy movement and collision handling
  - Score progression, levels, and win conditions
  - Freeze power-up for temporarily stopping enemies
  - Single-player save and resume support

- **Data structures and logic features**
  - **AVL tree** for theme storage and retrieval
  - **Min-heap leaderboard** for top-score management
  - **Queue-based matchmaking** for local bracket setup
  - File-based persistence for users, saves, themes, friends, and leaderboard data

- **UI / UX improvements in this version**
  - Save files are listed inside the **Load Saved Game** menu instead of requiring manual ID entry
  - Theme cards and menu layouts are cleaner and more consistent
  - Profile, settings, and friends screens are more readable
  - The project is split into clearer gameplay and menu state modules

## Tech stack

- **Language:** C++17
- **Graphics / windowing:** SFML
- **Build system:** CMake
- **Architecture:** Object-oriented, state-based menu and gameplay system

## Controls

### Gameplay
- **Player 1:** Arrow keys
- **Player 2:** `W A S D` (alternate bindings `Q G X` are also accepted in code for some directions)
- **Player 1 power-up:** `Space`
- **Player 2 power-up:** `Right Ctrl` or `Right Shift`
- **Save current single-player run:** `F5`
- **Back / leave match:** `Esc`

### Menus
- **Navigate save list:** `Up / Down`
- **Load selected save:** `Enter`
- **Theme shortcuts:** `1` to `0`
- **Register from login screen:** `Ctrl + R`

## Folder structure

```text
xonix-sfml-game/
├── CMakeLists.txt
├── README.md
├── .gitignore
├── main.cpp
├── Game*.cpp / Game*.h
├── Player.cpp / Player.h
├── Enemy.cpp / Enemy.h
├── SaveGame.cpp / SaveGame.h
├── Leaderboard.cpp / Leaderboard.h
├── Inventory.cpp / Inventory.h
├── Auth.cpp / Auth.h
├── UserDataIO.cpp / UserDataIO.h
├── ...
└── images/
    ├── enemy.png
    ├── gameover.png
    └── tiles.png
```

## Build instructions

### Option 1: Visual Studio (recommended for this project)
1. Open the folder as a **CMake project** in Visual Studio.
2. Install **SFML** and make sure CMake can find it.
3. If needed, update the SFML path inside `CMakeLists.txt`.
4. Build the `xonix` target.
5. Run the executable from the build output directory.

### Option 2: Command line with CMake
```bash
cmake -S . -B build
cmake --build build
```

If CMake cannot find SFML automatically, point `SFML_DIR` to your SFML CMake package directory before configuring.

## Font requirement

This project uses **Arial Black** (`ariblk.ttf`) for parts of the UI.

Due to licensing constraints, the font file is **not included** in this repository. Before running the game, place a valid local copy of `ariblk.ttf` in the project root, or update the font-loading path in the source code to use another font available on your system.


## Runtime data files

The game can generate the following files at runtime:

- `users.txt`
- `friends.txt`
- `leaderboard.txt`
- `user_themes.txt`
- `save_<id>.txt`

These are intentionally ignored in `.gitignore` so local gameplay data does not clutter the repository.
