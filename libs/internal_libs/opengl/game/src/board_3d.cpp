#include "objects/board_3d.h"
#include <iostream>

OpenGL::Game::Objects::Board3D::Board3D()
    : OpenGL::Geometry::Cuboid(glm::vec3(0.0f, 0.0f, 0.0f),  // At origin
                               glm::vec3(1.0f, 0.3f, 0.3f),  // size: wide, thin, shallow
                               "textures/atlas.png",         // atlas file
                               8, 48, 32, 16,                // sides texture region
                               209, 201, 26, 28)             // caps texture region
{
  std::cout << "Board3D Created" << std::endl;
  rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
  // Constructor delegates to Cuboid with board-specific dimensions
}

void OpenGL::Game::Objects::Board3D::draw(GLuint ShaderID, float deltaTime) {
  if (!ShaderID) {
    std::cerr << "ERROR: Shader ID is not set for Board3D::draw\n";
    return;
  }

  // 1) Handle input
  GLFWwindow* w = glfwGetCurrentContext();
  float movement = 0.0f;

  if (glfwGetKey(w, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS) {
    movement = -speed_ * deltaTime;
  }
  if (glfwGetKey(w, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS) {
    movement = +speed_ * deltaTime;
  }

  // 2) Apply movement if any key was pressed
  if (movement != 0.0f) {
    offsetX_ += movement;

    // Clamp to bounds
    const float minX = -1.5f;
    const float maxX = +1.5f;
    offsetX_ = glm::clamp(offsetX_, minX, maxX);

    // Get the cube's current transformation matrix
    glm::mat4 modelMatrix = getModelMatrix();  // Assuming Cuboid has this method

    // Extract the local X-axis (right direction) from the model matrix
    // The X-axis is the first column of the rotation part
    glm::vec3 localXAxis = glm::vec3(modelMatrix[0][0], modelMatrix[0][1], modelMatrix[0][2]);

    // Move along the cube's local X-axis (which points along the caps)
    glm::vec3 moveDirection = localXAxis * movement;
    translate(moveDirection);
  }

  // 3) Call parent's draw method
  OpenGL::Geometry::Cuboid::draw(ShaderID, deltaTime);
}