@echo off
REM Run CMake to configure the project
cmake ..
REM Build the project
cmake --build .
REM Enter the apps directory
cd apps
REM Run the game executable
.\my_game.exe
REM Return to the project root
cd ..