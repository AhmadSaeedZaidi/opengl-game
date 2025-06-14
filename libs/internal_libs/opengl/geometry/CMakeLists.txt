# Simple CMakeLists.txt for opengl_geometry
cmake_minimum_required(VERSION 3.16)

# Automatically find all header files in include/
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")

# Automatically find all source files in src/
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")

# Create the library
add_library(opengl_geometry STATIC ${SOURCES} ${HEADERS})

# Set include directory
target_include_directories(opengl_geometry PUBLIC include)

# Link dependencies - geometry only needs core, not graphics or game
target_link_libraries(opengl_geometry PUBLIC
    opengl_core
    glad
    glm
)
