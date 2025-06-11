#ifndef RENDER
#define RENDER
#include "window.h"
#include <shaders.h>

class RenderWindow : public WindowApp, public Shader {
  // RenderWindow class inherits from WindowApp and Shader
  // This class will handle the rendering of a rectangle using OpenGL
  // It uses the shader program created in the Shader class
 public:
  RenderWindow(int width, int height, const char* title, const char* vS, const char* fS);
  ~RenderWindow();
  void update() override;  // override the update method to render the rectangle
  void init() override;    // override the init method to set up OpenGL buffers

 private:
  unsigned int VAO = 0, VBO = 0, EBO = 0;

  // moved in from your free‐functions
  float vertices[24] = {
      // positions        // colors
      0.5f,  0.5f,  0.0f, 1, 0, 0,  // top right
      0.5f,  -0.5f, 0.0f, 0, 1, 0,  // bottom right
      -0.5f, -0.5f, 0.0f, 0, 0, 1,  // bottom left
      -0.5f, 0.5f,  0.0f, 1, 1, 0   // top left
  };
  unsigned int indices[6] = {0, 1, 3, 1, 2, 3};
};

#endif