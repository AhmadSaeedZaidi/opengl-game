#include "game.h"
#include <memory>
#include <vector>
#include <iostream>

OpenGL::Game::Game::Game(int width, int height, const char* title, const char* vertexShader,
                         const char* fragmentShader)
    : OpenGL::Core::WindowApp(width, height, title),
      shader_(vertexShader, fragmentShader),
      camera_(5.0f)  // Start camera 5 units away
{}

OpenGL::Game::Game::~Game() = default;

void OpenGL::Game::Game::addShape(std::unique_ptr<OpenGL::Geometry::Shape> shape) {
  shapes_.emplace_back(std::move(shape));
}

void OpenGL::Game::Game::init() {
  OpenGL::Core::WindowApp::init();

  // Enable depth testing for 3D
  glEnable(GL_DEPTH_TEST);

  // Initialize shader
  shader_.use();

  // Set texture uniforms
  shader_.setInt("sidesTexture", 0);
  shader_.setInt("capsTexture", 1);
  shader_.setBool("hasSidesTexture", true);
  shader_.setBool("hasCapsTexture", true);

  // Initialize all shapes
  std::cout << "Game::init() - Initializing " << shapes_.size() << " shapes" << std::endl;
  for (size_t i = 0; i < shapes_.size(); ++i) {
    std::cout << "Calling init() on shape " << i << std::endl;
    shapes_[i]->init();
    std::cout << "Shape " << i << " init() completed" << std::endl;
  }

  setupUniforms();
}

void OpenGL::Game::Game::update(float deltaTime) {
  // Process camera input
  camera_.processMouseInput(window);

  // Update shapes
}

void OpenGL::Game::Game::render(float deltaTime) {
  // Clear with depth buffer
  static int frameCount = 0;
  glClearColor(0.247f, 0.2824f, 0.8f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shader_.use();
  setupUniforms();

  if (frameCount++ % 60 == 0) {
    std::cout << "Shader ID: " << shader_.ID << std::endl;
    std::cout << "Drawing " << shapes_.size() << " shapes" << std::endl;
  }

  for (size_t i = 0; i < shapes_.size(); ++i) {
    if (frameCount % 60 == 0) {
      std::cout << "Calling draw() on shape " << i << std::endl;
    }
    shapes_[i]->draw(shader_.ID, deltaTime);

    // Check for OpenGL errors after drawing
    GLenum error = glGetError();
    if (error != GL_NO_ERROR && frameCount % 60 == 0) {
      std::cout << "OpenGL error after drawing shape " << i << ": " << error << std::endl;
    }
  }
}

void OpenGL::Game::Game::processInput() {
  // Base class escape key handling
  OpenGL::Core::WindowApp::processInput();

  // Camera input is handled in update()
}

void OpenGL::Game::Game::setupUniforms() {
  static int count = 0;
  // Set view and projection matrices
  float aspectRatio = (float)width / (float)height;
  glm::mat4 view = camera_.getViewMatrix();
  glm::mat4 projection = camera_.getProjectionMatrix(aspectRatio);
  if (count++ % 60 == 0) {  // Print every 60 frames
    std::cout << "Aspect ratio: " << aspectRatio << std::endl;
    // Print camera view matrix translation (position info)
    std::cout << "View matrix translation: " << view[3][0] << ", " << view[3][1] << ", "
              << view[3][2] << std::endl;
  }
  shader_.setMat4("view", view);
  shader_.setMat4("projection", projection);
}