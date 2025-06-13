#ifndef RENDER
#define RENDER
#include "window.h"
#include "textures.h"
#include <shaders.h>
#include <shape.h>
#include <vector>
#include <memory>

class RenderWindow : public WindowApp, public Shader {
  // RenderWindow class inherits from WindowApp and Shader
  // This class will handle the rendering of a rectangle using OpenGL
  // It uses the shader program created in the Shader class
 public:
  RenderWindow(int width, int height, const char* title, const char* vS, const char* fS);
  ~RenderWindow();
  // add any shape
  void addShape(std::unique_ptr<Shape> s);

 protected:
  void update(float deltaTime) override;  // override the update method to render the rectangle
  void init() override;    // override the init method to set up OpenGL buffers
 private:
  std::vector<std::unique_ptr<Shape>> shapes;
};

#endif