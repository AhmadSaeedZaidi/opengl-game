# Simple CMakeLists.txt for opengl_graphics
cmake_minimum_required(VERSION 3.16)

# Automatically find all header files in include/
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")

# Automatically find all source files in src/
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")

# Create the library
add_library(opengl_graphics STATIC ${SOURCES} ${HEADERS})

# Set include directory
target_include_directories(opengl_graphics PUBLIC include)

# Link dependencies - graphics only needs core, not geometry or game
target_link_libraries(opengl_graphics PUBLIC
    opengl_core
    glad
    glfw
    glm
)
