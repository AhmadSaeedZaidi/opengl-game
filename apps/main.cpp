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
      -0.2f, -0.95f,  // bottom-left
      0.2f, -0.8f     // top-right
  };
  auto s = std::make_unique<Rectangle>(board_coords, "textures/board.png");
  window->addShape(std::move(s));
  window->run();  // run the window application
  return 0;
}
