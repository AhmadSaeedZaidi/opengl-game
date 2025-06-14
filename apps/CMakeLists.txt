add_executable(my_game main.cpp)
target_link_libraries(my_game PRIVATE
    opengl_core
    opengl_graphics  
    opengl_geometry
    opengl_game
    glad
    glfw
    glm
)