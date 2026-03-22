cmake_minimum_required(VERSION 3.16)

project(Xonix)

# Tell CMake where SFML is installed
set(SFML_DIR "C:/SFML/lib/cmake/SFML")

find_package(SFML REQUIRED network audio graphics window system)

# Copy images folder next to the built .exe
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/images"
     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")

add_executable(xonix main.cpp "Player.cpp" "Player.h" "Enemy.h" "Enemy.cpp" "Auth.h" "Auth.cpp" "GameLogic.h" "GameLogic.cpp" "State.h" "GameData.h" "Game.h" "Game.cpp" "LoginState.h" "LoginState.cpp" "MainMenuState.h" "MainMenuState.cpp" "GameState.h" "GameState.cpp" "SinglePlayerMenuState.h" "SinglePlayerMenuState.cpp" "State.cpp" "MultiplayerMenuState.cpp" "GameRoomState.cpp" "Leaderboard.h" "Leaderboard.cpp" "LeaderboardState.h" "LeaderboardState.cpp" "ProfileState.h" "ProfileState.cpp" "MatchmakingQueue.h" "MatchmakingQueue.cpp" "MatchResultState.h" "MatchResultState.cpp" "Inventory.h" "Inventory.cpp" "ThemeMenuState.h" "ThemeMenuState.cpp")

target_link_libraries(xonix PRIVATE
    sfml-system
    sfml-window
    sfml-graphics
    sfml-network
    sfml-audio
)
