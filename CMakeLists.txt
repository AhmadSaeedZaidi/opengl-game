# top-level cmake
cmake_minimum_required(VERSION 3.16)  # Bumped for better C++17 support
project(OpenGLProject)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    # Check if we're using MinGW and set proper environment
    if(MINGW)
        set(ENV{CCACHE_CC} "gcc")
        set(ENV{CCACHE_CXX} "g++")
    endif()
    
    set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    message(STATUS "ccache found and configured: ${CCACHE_PROGRAM}")
else()
    message(STATUS "ccache not found")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

# Find shader files
file(GLOB SHADER_FILES "${CMAKE_SOURCE_DIR}/shaders/*.vert" "${CMAKE_SOURCE_DIR}/shaders/*.frag")
file(GLOB TEXTURE_FILES
     "${CMAKE_SOURCE_DIR}/textures/*.png"
     "${CMAKE_SOURCE_DIR}/textures/*.jpg"
     # add more extensions if needed
)

# Create shader and texture directories in build folder
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/apps/shaders)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/apps/textures)

# EXPLICITLY copy each shader file (more reliable than directory copy)
foreach(SHADER_FILE ${SHADER_FILES})
    get_filename_component(SHADER_FILENAME ${SHADER_FILE} NAME)
    configure_file(${SHADER_FILE} ${CMAKE_BINARY_DIR}/apps/shaders/${SHADER_FILENAME} COPYONLY)
endforeach()

# Copy texture files
foreach(TEXT_FILE ${TEXTURE_FILES})
  get_filename_component(TEXT_NAME ${TEXT_FILE} NAME)
  configure_file(
    ${TEXT_FILE}
    ${CMAKE_BINARY_DIR}/apps/textures/${TEXT_NAME}
    COPYONLY
  )
endforeach()

# Add subdirectories
add_subdirectory(libs)
add_subdirectory(apps)
