# top-level cmake
cmake_minimum_required(VERSION 3.10)
project(OpenGLProject)

enable_testing()

# In root CMakeLists.txt
# Find shader files
file(GLOB SHADER_FILES "${CMAKE_SOURCE_DIR}/shaders/*.vert" "${CMAKE_SOURCE_DIR}/shaders/*.frag")

# Create shader directory in build folder
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/apps/shaders)

# EXPLICITLY copy each shader file (more reliable than directory copy)
foreach(SHADER_FILE ${SHADER_FILES})
    get_filename_component(SHADER_FILENAME ${SHADER_FILE} NAME)
    configure_file(${SHADER_FILE} ${CMAKE_BINARY_DIR}/apps/shaders/${SHADER_FILENAME} COPYONLY)
endforeach()

add_subdirectory(libs)
add_subdirectory(apps)
