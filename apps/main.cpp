#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <render.h>
#include <rectangle.h>
#include <textures.h>

int main() {
  auto window = std::make_unique<RenderWindow>(800, 600, "LearnOpenGL", "shaders/basic.vert",
                                               "shaders/basic.frag");

  float board_coords[] = {
      // x,   y
      0.5f, 0.1f,  // Top-right
      -0.5f, 0.0f  // Bottom-left
  };
  window->addShape(
      std::make_unique<Rectangle>(board_coords, "textures/breakout.png", 0, 336, 64, 16));
  window->run();  // run the window application
  return 0;
}
