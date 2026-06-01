#include "objects/brick.h"
#include "managers/collision_manager.h"  // Include here to get full definition
#include "log.h"
#include <iostream>

OpenGL::Game::Objects::Brick::Brick(const glm::vec3& position, OpenGL::Core::TextureAtlas& atlas,
                                   std::string sidesRegion, std::string capsRegion,
                                   const glm::vec3& color)
    : OpenGL::Geometry::Cylinder(position,
                                 BRICK_HEIGHT / 2.0f,   // radius (half height for thin cylinder)
                                 BRICK_WIDTH,           // length along X-axis
                                 atlas,                 // shared texture atlas
                                 std::move(sidesRegion),
                                 std::move(capsRegion),
                                 8),  // 8 segments (sufficient for brick)
      destroyed_(false),
      color_(color) {
#if OPENGL_VERBOSE_LOG
  std::cout << "Brick Created at position: " << position.x << ", " << position.y << ", "
            << position.z << std::endl;
#endif
}

void OpenGL::Game::Objects::Brick::draw(GLuint shaderID, float deltaTime, GLFWwindow* window) {
  if (destroyed_) return;  // Don't draw if destroyed

  // Call parent's draw method
  OpenGL::Geometry::Cylinder::draw(shaderID, deltaTime, window);
}

// Implement the collision box method here (moved from header)
OpenGL::Game::Managers::BoundingBox2D OpenGL::Game::Objects::Brick::getCollisionBox() const {
  glm::vec3 pos = getPosition();
  return OpenGL::Game::Managers::BoundingBox2D(pos.x, pos.y, BRICK_WIDTH, BRICK_HEIGHT);
}

glm::vec3 OpenGL::Game::Objects::Brick::getMin() const {
  glm::vec3 pos = getPosition();
  return glm::vec3(pos.x - BRICK_WIDTH / 2.0f, pos.y - BRICK_HEIGHT / 2.0f,
                   pos.z - BRICK_DEPTH / 2.0f);
}

glm::vec3 OpenGL::Game::Objects::Brick::getMax() const {
  glm::vec3 pos = getPosition();
  return glm::vec3(pos.x + BRICK_WIDTH / 2.0f, pos.y + BRICK_HEIGHT / 2.0f,
                   pos.z + BRICK_DEPTH / 2.0f);
}