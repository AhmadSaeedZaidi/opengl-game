#!/usr/bin/env bash
# Runs the game. Assumes ./install.sh has been run at least once.

set -euo pipefail

BUILD_DIR="build"
PROJECT_ROOT="$(cd "$(dirname "$0")" && pwd)"
cd "$PROJECT_ROOT"

if [ ! -x "$BUILD_DIR/apps/my_game" ]; then
    echo "error: $BUILD_DIR/apps/my_game not found." >&2
    echo "       run ./install.sh first." >&2
    exit 1
fi

cd "$BUILD_DIR/apps"
exec ./my_game
