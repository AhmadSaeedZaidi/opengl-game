#include "window.h"
#include <iostream>

OpenGL::Core::WindowApp::WindowApp(int width, int height, const char* title)
    : width(width), height(height), title(title) {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW\n";
    return;
  }
  glfwInitialized_ = true;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(width, height, title, nullptr, nullptr);

  if (!window) {
    std::cerr << "Failed to create GLFW window\n";
    return;
  }
  glfwSetWindowPos(window, 720, 50);
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD\n";
    glfwDestroyWindow(window);
    window = nullptr;
    return;
  }

  glViewport(0, 0, width, height);
}

OpenGL::Core::WindowApp::~WindowApp() {
  if (window) {
    glfwDestroyWindow(window);
    window = nullptr;
  }
  if (glfwInitialized_) {
    glfwTerminate();
    glfwInitialized_ = false;
  }
}

void OpenGL::Core::WindowApp::run() {
  init();

  if (!window) {
    std::cerr << "Window initialization failed\n";
    return;
  }

  glfwSwapInterval(1);

  double last = glfwGetTime();
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    double now = glfwGetTime();
    float delta = static_cast<float>(now - last);
    last = now;

    if (delta > 0.1f) {
      delta = 0.1f;
    }

    processInput();
    update(delta);
    render(delta);
    glfwSwapBuffers(window);
  }
}

void OpenGL::Core::WindowApp::render(float deltaTime) {
  // default: clear screen
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGL::Core::WindowApp::processInput() {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void OpenGL::Core::WindowApp::update(float deltaTime) {
  // default: do nothing
}

void OpenGL::Core::WindowApp::init() {}