#include "camera.h"
#include <iostream>

OpenGL::Graphics::OrbitCamera::OrbitCamera(float distance)
    : distance_(distance),
      yaw_(0.0f),
      pitch_(0.0f),
      dragging_(false),
      lastMouseX_(0.0),
      lastMouseY_(0.0),
      sensitivity_(0.5f),
      target_(0.0f, 0.0f, 0.0f),
      up_(0.0f, 1.0f, 0.0f) {
  updatePosition();
}

void OpenGL::Graphics::OrbitCamera::processMouseInput(GLFWwindow* window) {
  double mouseX, mouseY;
  glfwGetCursorPos(window, &mouseX, &mouseY);

  // Check for mouse button press/release
  bool leftPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

  if (leftPressed && !dragging_) {
    // Start dragging
    dragging_ = true;
    lastMouseX_ = mouseX;
    lastMouseY_ = mouseY;
  } else if (!leftPressed && dragging_) {
    // Stop dragging
    dragging_ = false;
  }

  // If dragging, update camera angles
  if (dragging_) {
    double deltaX = mouseX - lastMouseX_;
    double deltaY = mouseY - lastMouseY_;

    yaw_ += deltaX * sensitivity_;
    pitch_ -= deltaY * sensitivity_;  // Negative because screen Y is flipped

    // Clamp pitch to prevent camera from flipping
    if (pitch_ > 89.0f) pitch_ = 89.0f;
    if (pitch_ < -89.0f) pitch_ = -89.0f;

    lastMouseX_ = mouseX;
    lastMouseY_ = mouseY;

    updatePosition();
  }

  // Reset camera with R key
  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
    reset();
  }
}

void OpenGL::Graphics::OrbitCamera::reset() {
  yaw_ = 0.0f;
  pitch_ = 0.0f;
  updatePosition();
}

void OpenGL::Graphics::OrbitCamera::updatePosition() {
  // Convert spherical coordinates to Cartesian
  float x = distance_ * cos(glm::radians(pitch_)) * cos(glm::radians(yaw_));
  float y = distance_ * sin(glm::radians(pitch_));
  float z = distance_ * cos(glm::radians(pitch_)) * sin(glm::radians(yaw_));

  // Position camera in positive Z direction by default (when yaw=0, pitch=0)
  position_ = glm::vec3(x, y, z);
}

glm::mat4 OpenGL::Graphics::OrbitCamera::getViewMatrix() {
  return glm::lookAt(position_, target_, up_);
}

glm::mat4 OpenGL::Graphics::OrbitCamera::getProjectionMatrix(float aspectRatio) {
  return glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
}