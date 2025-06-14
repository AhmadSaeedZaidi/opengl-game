file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

add_library(game ${SOURCES} ${HEADERS})
target_include_directories(game PUBLIC include)
target_link_libraries(game PRIVATE glad glfw glm shape window shaders)
