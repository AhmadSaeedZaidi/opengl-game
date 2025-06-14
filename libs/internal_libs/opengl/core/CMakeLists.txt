# Simple CMakeLists.txt for opengl_core
cmake_minimum_required(VERSION 3.16)

# Automatically find all header files in include/
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")

# Automatically find all source files in src/
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")

# Create the library
add_library(opengl_core STATIC ${SOURCES} ${HEADERS})

# Set include directory
target_include_directories(opengl_core PUBLIC include)

target_include_directories(opengl_core PUBLIC 
    include
    ../../../external_libs/stb_image  # ← ADD THIS for stb_image.h
)

# Link dependencies
target_link_libraries(opengl_core PUBLIC
    glm
    glad
    glfw
    stb_image
)
