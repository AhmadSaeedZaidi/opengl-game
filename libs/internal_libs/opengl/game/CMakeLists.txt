# Simple CMakeLists.txt for opengl_core
cmake_minimum_required(VERSION 3.16)

# Automatically find all header files in include/
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")

# Automatically find all source files in src/
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")

# Create the library
add_library(opengl_game STATIC ${SOURCES} ${HEADERS})

# Set include directory
target_include_directories(opengl_game PUBLIC include)

# Link dependencies
target_link_libraries(opengl_game PUBLIC
    opengl_core
    opengl_graphics
    opengl_geometry
    glad
    glfw
    glm
)