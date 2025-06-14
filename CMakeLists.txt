# top-level cmake
cmake_minimum_required(VERSION 3.16)  # Bumped for better C++17 support
project(OpenGLProject)

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
