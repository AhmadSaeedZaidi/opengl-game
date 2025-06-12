#include <glad/glad.h>
#include <render.h>
#include <iostream>

RenderWindow::RenderWindow(int width, int height, const char* title, const char* vS,
                           const char* fS)
    : WindowApp(width, height, title), Shader(vS, fS) {}

void RenderWindow::addShape(std::unique_ptr<Shape> s) { shapes.emplace_back(std::move(s)); }

void RenderWindow::update() {
  Shader::use();
  for (auto& s : shapes) s->draw();
}

void RenderWindow::init() {
  std::cout << "Test" << std::endl;
  WindowApp::init();  // call the base class init method
  Shader::use();
  std::cout << "Test 2" << std::endl;
  Shader::setInt("ourTexture", 0);
  for (auto& s : shapes) s->init();
}

RenderWindow::~RenderWindow() = default;