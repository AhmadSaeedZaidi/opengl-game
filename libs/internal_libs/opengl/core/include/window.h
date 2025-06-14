#ifndef WINDOW
#define WINDOW
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <string>
#include <fstream>
namespace OpenGL::Core {
class WindowApp {
 public:
  WindowApp(int width, int height, const char* title);
  ~WindowApp();
  void run();

 protected:
  virtual void render(float deltaTime);
  virtual void update(float deltaTime);
  virtual void init();
  virtual void processInput();
  GLFWwindow* window;
  int height, width;
  const char* title;
};
}  // namespace OpenGL::Core
#endif