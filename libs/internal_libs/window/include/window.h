#ifndef WINDOW
#define WINDOW
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <string>
#include <fstream>

class WindowApp {
 public:
  WindowApp(int width, int height, const char* title);
  ~WindowApp();
  void run();

 protected:
  virtual void render();
  virtual void update();
  virtual void init();
  virtual void processInput();
  GLFWwindow* window;
  int height, width;
};

#endif