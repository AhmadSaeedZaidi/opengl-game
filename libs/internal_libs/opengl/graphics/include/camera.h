#ifndef ORBIT_CAMERA_H
#define ORBIT_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
namespace OpenGL::Graphics {
class OrbitCamera {
 public:
  OrbitCamera(float distance = 5.0f);

  // Call these from your main loop
  void processMouseInput(GLFWwindow* window);
  void reset();

  // Get matrices for your shaders
  glm::mat4 getViewMatrix();
  glm::mat4 getProjectionMatrix(float aspectRatio);

 private:
  // Camera parameters
  float distance_;  // Distance from center
  float yaw_;       // Horizontal angle
  float pitch_;     // Vertical angle

  // Mouse control
  bool dragging_;
  double lastMouseX_, lastMouseY_;
  float sensitivity_;

  // Camera vectors
  glm::vec3 position_;
  glm::vec3 target_;
  glm::vec3 up_;

  void updatePosition();
};
}  // namespace OpenGL::Graphics
#endif