#!/usr/bin/env bash
# Builds the OpenGL game on Linux/macOS. Run once, then use ./play.sh to run.
# Mirrors install.bat for Windows.

set -euo pipefail

BUILD_DIR="build"
PROJECT_ROOT="$(cd "$(dirname "$0")" && pwd)"
cd "$PROJECT_ROOT"

if [ -d "$BUILD_DIR" ]; then
    rm -rf "$BUILD_DIR"
fi

# Compiler cache: significantly speeds up rebuilds by reusing previously
# compiled object files. Matches install.bat on Windows. Skipped silently
# if ccache is not installed; disable at runtime with CCACHE_DISABLE=1.
CCACHE_ARGS=()
if command -v ccache >/dev/null 2>&1; then
    CCACHE_ARGS=(-DCMAKE_C_COMPILER_LAUNCHER=ccache -DCMAKE_CXX_COMPILER_LAUNCHER=ccache)
    echo "ccache found: $(ccache --version | head -n1)"
else
    echo "warning: ccache not found; builds will not be cached." >&2
    echo "         install it via your package manager to speed up rebuilds." >&2
fi

cmake -B "$BUILD_DIR" -G "Unix Makefiles" "${CCACHE_ARGS[@]}"
cmake --build "$BUILD_DIR" -j"$(nproc)"

echo
echo "Build complete. Run ./play.sh to start the game."

if command -v ccache >/dev/null 2>&1; then
    echo
    echo "ccache stats:"
    ccache --show-stats
fi
