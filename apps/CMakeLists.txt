add_executable(my_game main.cpp)
target_link_libraries(my_game PRIVATE glad glfw shaders)
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR}/apps)