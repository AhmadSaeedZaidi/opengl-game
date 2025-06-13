#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <render.h>
#include <rectangle.h>
#include <textures.h>
#include <board.h>

int main() {
  auto window = std::make_unique<RenderWindow>(800, 600, "LearnOpenGL", "shaders/basic.vert",
                                               "shaders/basic.frag");
  window->addShape(std::make_unique<Board>());
  window->run();  // run the window application
  return 0;
}
