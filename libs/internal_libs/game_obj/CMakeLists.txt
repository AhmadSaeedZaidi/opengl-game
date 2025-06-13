file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

add_library(game_obj ${SOURCES} ${HEADERS})
target_include_directories(game_obj PUBLIC include)
target_link_libraries(game_obj PRIVATE glad glfw glm shape window shaders)
