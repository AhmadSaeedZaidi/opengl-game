#include "window.h"
#include <glfw/glfw3.h>
#include <glad/glad.h>
#include <iostream>

WindowApp::WindowApp(int width, int height, const char* title)
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
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD\n";
    glfwDestroyWindow(window);
    glfwTerminate();
    return;
  }

  glViewport(0, 0, width, height);
}

WindowApp::~WindowApp() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

void WindowApp::run() {
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
}

void WindowApp::render(float deltaTime) {
  // default: clear screen
  glClearColor(0.247f, 0.2824f, 0.8f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void WindowApp::processInput() {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void WindowApp::update(float deltaTime) {
  // Default: do nothing
}

void WindowApp::init() {}