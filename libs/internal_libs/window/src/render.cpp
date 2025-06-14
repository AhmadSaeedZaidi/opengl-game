#include "render.h"
#include "window.h"
#include "textures.h"
#include <shaders.h>
#include <shape.h>
#include <vector>
#include <memory>

RenderWindow::RenderWindow(int width, int height, const char* title, const char* vS,
                           const char* fS)
    : WindowApp(width, height, title), Shader(vS, fS) {}

void RenderWindow::addShape(std::unique_ptr<Shape> s) { shapes.emplace_back(std::move(s)); }

void RenderWindow::update(float deltaTime) {
  Shader::use();
  for (auto& s : shapes) s->draw(Shader::ID, deltaTime);
}

void RenderWindow::init() {
  WindowApp::init();  // call the base class init method
  Shader::use();
  Shader::setInt("ourTexture", 0);
  for (auto& s : shapes) s->init();
}

RenderWindow::~RenderWindow() = default;