@echo off
echo Building OpenGL Game with ccache...

REM Set environment
set PATH=C:\msys64\mingw64\bin;C:\msys64\usr\bin;%PATH%
set CCACHE_CC=gcc
set CCACHE_CXX=g++

REM Clean and build
if exist build rmdir /s /q build
cmake -B build -G "MinGW Makefiles"
mingw32-make -C build -j%NUMBER_OF_PROCESSORS%

REM Show ccache stats
echo.
echo ccache stats:
ccache --show-stats

REM Run game
cd build/apps
.\my_game.exe
pause