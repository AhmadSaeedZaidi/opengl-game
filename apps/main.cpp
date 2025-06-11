#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <render.h>

int main() {
  RenderWindow* window;  // create a window instance
  window = new RenderWindow(800, 600, "LearnOpenGL", "shaders/basic.vert", "shaders/basic.frag");
  window->run();  // run the window application
  return 0;
}
