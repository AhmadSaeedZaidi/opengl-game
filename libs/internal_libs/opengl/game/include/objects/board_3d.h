#ifndef BOARD_3D
#define BOARD_3D

#include <iostream>
#include <cylinder.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <algorithm>

// Forward declaration for collision support
namespace OpenGL::Game::Managers {
struct BoundingBox2D;
}

namespace OpenGL::Game::Objects {
class Board3D : public OpenGL::Geometry::Cylinder {
 public:
  Board3D();
  void draw(GLuint ShaderID, float deltaTime) override;

  // Collision support for managers
  Managers::BoundingBox2D getCollisionBox() const;

  // Speed buff methods
  void onBallHit();
  void resetSpeed();
  float getCurrentSpeed() const { return currentSpeed_; }

 private:
  const float speed_ = 2.0f;
  float currentSpeed_ = 2.0f;                         // Current movement speed
  int paddleHits_ = 0;                                // Track hits for speed buff
  static constexpr float SPEED_BOOST_PER_HIT = 0.1f;  // Speed increase per hit
  static constexpr float MAX_SPEED = 4.0f;            // Cap the speed

  // Board dimensions for collision
  static constexpr float BOARD_WIDTH = 0.8f;
  static constexpr float BOARD_HEIGHT = 0.15f;
};
}  // namespace OpenGL::Game::Objects
#endif  // BOARD_3D