#include "window.h"
#include <iostream>

OpenGL::Core::WindowApp::WindowApp(int width, int height, const char* title)
    : width(width), height(height), title(title) {
  if (!glfwInit()) {
    std::cout << "Failed to initialize GLFW\n";
    return;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(width, height, title, nullptr, nullptr);

  if (!window) {
    std::cout << "Failed to create GLFW window\n";
    glfwTerminate();
    return;
  }
  glfwSetWindowPos(window, 720, 50);
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD\n";
    glfwDestroyWindow(window);
    glfwTerminate();
    return;
  }

  glViewport(0, 0, width, height);
}

OpenGL::Core::WindowApp::~WindowApp() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

void OpenGL::Core::WindowApp::run() {
  init();  // call the init method to set up the window and OpenGL context

  if (!window) {
    std::cout << "Window initialization failed\n";
    return;
  }
  double last = glfwGetTime();
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    double now = glfwGetTime();
    float delta = float(now - last);
    last = now;
    processInput();
    render(delta);
    update(delta);
    glfwSwapBuffers(window);
  }
  glfwDestroyWindow(window);
  glfwTerminate();
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