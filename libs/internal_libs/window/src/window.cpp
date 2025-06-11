#include "window.h"
#include <glfw/glfw3.h>
#include <glad/glad.h>

WindowApp::WindowApp(int width, int height, const char* title) : width(width), height(height) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  window = glfwCreateWindow(width, height, title, nullptr, nullptr);

  if (window == nullptr) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
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
  while (!glfwWindowShouldClose(window)) {
    processInput();
    render();
    update();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

void WindowApp::render() {
  // default: clear screen
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

void WindowApp::processInput() {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void WindowApp::update() {
  // default: do nothing
}

void WindowApp::init() {
  // default: do nothing
}