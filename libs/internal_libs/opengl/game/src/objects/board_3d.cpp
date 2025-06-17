#include "objects/board_3d.h"
#include "managers/collision_manager.h"  // Add this for collision support
#include <iostream>

OpenGL::Game::Objects::Board3D::Board3D()
    : OpenGL::Geometry::Cylinder(
          glm::vec3(0.0f, -1.5f, 0.0f),  // Move down on Y-axis
          BOARD_HEIGHT * 0.5f,           // radius = half the height for thin cylinder
          BOARD_WIDTH,                   // height (length) = board width
          "textures/atlas.png",          // atlas file
          0, 0, 384, 512,                // sides texture region (rotated 90°)
          0, 512, 384, 512,              // caps texture region (end caps)
          16)                            // 16 segments for smoothness
{
  std::cout << "Board3D Created (Cylinder) - Width:" << BOARD_WIDTH << " Height:" << BOARD_HEIGHT
            << std::endl;
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
    // Get current position
    glm::vec3 currentPos = getPosition();

    // Calculate new X position
    float newX = currentPos.x + movement;

    // Clamp to bounds using board width for proper edge calculation
    const float boardHalfWidth = BOARD_WIDTH * 0.5f;
    const float minX = -2.0f + boardHalfWidth;  // World bound + half board width
    const float maxX = 2.0f - boardHalfWidth;   // World bound - half board width
    newX = glm::clamp(newX, minX, maxX);

    // Set the new position
    setPosition(glm::vec3(newX, currentPos.y, currentPos.z));
  }

  // 3) Call parent's draw method
  OpenGL::Geometry::Cylinder::draw(ShaderID, deltaTime);
}

// Collision support method for managers
OpenGL::Game::Managers::BoundingBox2D OpenGL::Game::Objects::Board3D::getCollisionBox() const {
  glm::vec3 pos = getPosition();
  return OpenGL::Game::Managers::BoundingBox2D(pos.x, pos.y, BOARD_WIDTH, BOARD_HEIGHT);
}