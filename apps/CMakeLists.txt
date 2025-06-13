add_executable(my_game main.cpp)
target_link_libraries(my_game PRIVATE glad glfw window shaders stb_image shape game_obj glm) 