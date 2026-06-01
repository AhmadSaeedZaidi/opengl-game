#include "objects/ball.h"
#include "managers/collision_manager.h"
#include "log.h"
#include <iostream>
#include <algorithm>
#include <cmath>

OpenGL::Game::Objects::Ball::Ball(const glm::vec3& position, OpenGL::Core::TextureAtlas& atlas,
                                 std::string regionName, float radius)
    : OpenGL::Geometry::Sphere(position, radius, atlas, std::move(regionName),
                               8,   // latitude segments
                               8),  // longitude segments
      velocity2D_(1.0f, 2.0f),    // Start with some velocity
      radius_(radius),
      startPosition_(position) {  // Store start position
#if OPENGL_VERBOSE_LOG
  std::cout << "Ball Created at position: " << position.x << ", " << position.y << ", "
            << position.z << std::endl;
#endif
}

void OpenGL::Game::Objects::Ball::update(float deltaTime) {
  // Apply gravity (only in Y direction)
  // velocity2D_.y += GRAVITY_2D * deltaTime;

  // Limit speed to prevent crazy physics
  float speed = sqrt(velocity2D_.x * velocity2D_.x + velocity2D_.y * velocity2D_.y);
  if (speed > SPEED_LIMIT) {
    velocity2D_.x = (velocity2D_.x / speed) * SPEED_LIMIT;
    velocity2D_.y = (velocity2D_.y / speed) * SPEED_LIMIT;
  }

  // Update position (only X/Y, keep Z constant)
  glm::vec3 currentPos = getPosition();
  glm::vec3 newPosition =
      glm::vec3(currentPos.x + velocity2D_.x * deltaTime, currentPos.y + velocity2D_.y * deltaTime,
                currentPos.z  // Keep Z unchanged
      );
  setPosition(newPosition);

  // NOTE: Boundary checking removed - now handled by CollisionManager
  // NOTE: Collision detection removed - now handled by CollisionManager
}

void OpenGL::Game::Objects::Ball::draw(GLuint shaderID, float deltaTime, GLFWwindow* window) {
  // Call parent's draw method
  OpenGL::Geometry::Sphere::draw(shaderID, deltaTime, window);
}

void OpenGL::Game::Objects::Ball::resetToStart() {
  setPosition(startPosition_);
  setVelocity2D(glm::vec2(1.0f, 2.0f));  // Reset with initial velocity
#if OPENGL_VERBOSE_LOG
  std::cout << "Ball reset to start position!" << std::endl;
#endif
}

// Now define the method that returns Circle2D (was inline in header)
OpenGL::Game::Managers::Circle2D OpenGL::Game::Objects::Ball::getCollisionCircle() const {
  glm::vec3 pos = getPosition();
  return OpenGL::Game::Managers::Circle2D(pos.x, pos.y, radius_);
}