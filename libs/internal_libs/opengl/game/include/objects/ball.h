#ifndef OPENGL_GAME_OBJECTS_BALL_H
#define OPENGL_GAME_OBJECTS_BALL_H

#include "sphere.h"
#include <glm/glm.hpp>

namespace OpenGL::Game::Managers {
struct Circle2D;
}

namespace OpenGL::Game::Objects {

class Ball : public OpenGL::Geometry::Sphere {
 public:
  Ball(const glm::vec3& position, float radius = 0.05f);
  ~Ball() = default;

  void update(float deltaTime);
  void draw(GLuint shaderID, float deltaTime) override;

  // 2D Physics (X/Y plane only)
  void setVelocity2D(const glm::vec2& velocity) { velocity2D_ = velocity; }
  glm::vec2 getVelocity2D() const { return velocity2D_; }
  void addVelocity2D(const glm::vec2& deltaV) { velocity2D_ += deltaV; }

  // Collision support methods (defined in .cpp to avoid circular include)
  Managers::Circle2D getCollisionCircle() const;
  float getRadius() const { return radius_; }
  void resetToStart();

  // Bounce mechanics
  void bounceX() { velocity2D_.x = -velocity2D_.x; }
  void bounceY() { velocity2D_.y = -velocity2D_.y; }
  void setBounceVelocity(const glm::vec2& newVel) { velocity2D_ = newVel; }

 private:
  glm::vec2 velocity2D_;  // Only X/Y movement
  float radius_;
  glm::vec3 startPosition_;

  static constexpr float GRAVITY_2D = -9.81f * 0.1f;
  static constexpr float BOUNCE_DAMPING = 0.95f;
  static constexpr float SPEED_LIMIT = 5.0f;
};

}  // namespace OpenGL::Game::Objects

#endif  // OPENGL_GAME_OBJECTS_BALL_H