cmake_minimum_required(VERSION 3.16)
project(Xonix)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32 AND EXISTS "C:/SFML/lib/cmake/SFML")
    set(SFML_DIR "C:/SFML/lib/cmake/SFML")
endif()

find_package(SFML REQUIRED COMPONENTS system window graphics audio)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/images" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/ariblk.ttf" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")

add_executable(xonix
    main.cpp
    Auth.cpp
    Enemy.cpp
    FriendsManager.cpp
    Game.cpp
    GameLogic.cpp
    GameRoomState.cpp
    GameState.cpp
    Inventory.cpp
    Leaderboard.cpp
    LeaderboardState.cpp
    LoginState.cpp
    MainMenuState.cpp
    MatchmakingQueue.cpp
    MatchResultState.cpp
    MultiplayerMenuState.cpp
    Player.cpp
    ProfileState.cpp
    SaveGame.cpp
    SettingsState.cpp
    SinglePlayerMenuState.cpp
    State.cpp
    ThemeMenuState.cpp
    UserDataIO.cpp
    FriendsState.cpp
)

target_link_libraries(xonix PRIVATE sfml-system sfml-window sfml-graphics sfml-audio)
