cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(minesweeper)

# Find SFML
set(SFML_DIR "/lib/cmake/SFML")
find_package(SFML 2.5.1 COMPONENTS system window graphics audio REQUIRED)

# Include directories
include_directories(include)

# Add the executable
add_executable(minesweeper
    main.cpp
    Config.cpp
    Game.cpp
    WelcomeScreen.cpp
    Singleton.cpp
)

# Link SFML libraries
target_link_libraries(minesweeper sfml-system sfml-window sfml-graphics sfml-audio)