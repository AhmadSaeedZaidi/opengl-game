#!/usr/bin/env bash
# Builds the OpenGL game on Linux/macOS using cmake and make.
# Mirrors play.bat for Windows.

set -euo pipefail

BUILD_DIR="build"
PROJECT_ROOT="$(cd "$(dirname "$0")" && pwd)"
cd "$PROJECT_ROOT"

if [ -d "$BUILD_DIR" ]; then
    rm -rf "$BUILD_DIR"
fi

cmake -B "$BUILD_DIR" -G "Unix Makefiles"
make -C "$BUILD_DIR" -j"$(nproc)"

cd "$BUILD_DIR/apps"
./my_game
