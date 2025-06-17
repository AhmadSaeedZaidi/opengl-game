#ifndef OPENGL_GAME_MANAGERS_COLLISION_MANAGER_H
#define OPENGL_GAME_MANAGERS_COLLISION_MANAGER_H

#include "../objects/ball.h"
#include "../objects/brick.h"
#include "../objects/board_3d.h"
#include "game_state_manager.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace OpenGL::Game::Managers {

// Move the collision structures into the manager
struct BoundingBox2D {
  float x, y;           // Center position
  float width, height;  // Dimensions

  BoundingBox2D(float centerX, float centerY, float w, float h)
      : x(centerX), y(centerY), width(w), height(h) {}

  // Get corners
  float left() const { return x - width * 0.5f; }
  float right() const { return x + width * 0.5f; }
  float top() const { return y + height * 0.5f; }
  float bottom() const { return y - height * 0.5f; }
};

struct Circle2D {
  float x, y;    // Center position
  float radius;  // Radius

  Circle2D(float centerX, float centerY, float r) : x(centerX), y(centerY), radius(r) {}
};

class CollisionManager {
 public:
  CollisionManager(GameStateManager& gameState);

  // Main collision checking method
  void checkAllCollisions(Objects::Ball* ball, Objects::Board3D* board,
                          std::vector<std::unique_ptr<Objects::Brick>>& bricks);

  // Individual collision checks
  void checkBallBrickCollisions(Objects::Ball* ball,
                                std::vector<std::unique_ptr<Objects::Brick>>& bricks);

  void checkBallBoardCollisions(Objects::Ball* ball, Objects::Board3D* board);

  void checkBallBoundaries(Objects::Ball* ball);

  // Boundary settings
  void setBoundaries(float minX, float maxX, float minY, float maxY);

  // Get boundary info
  float getMinX() const { return WORLD_MIN_X; }
  float getMaxX() const { return WORLD_MAX_X; }
  float getMinY() const { return WORLD_MIN_Y; }
  float getMaxY() const { return WORLD_MAX_Y; }

 private:
  // Collision detection methods
  bool circleRect(const Circle2D& circle, const BoundingBox2D& rect);
  bool circleCircle(const Circle2D& c1, const Circle2D& c2);
  bool rectRect(const BoundingBox2D& r1, const BoundingBox2D& r2);

  // Physics calculations
  glm::vec2 calculateBounce(const Circle2D& circle, const glm::vec2& velocity,
                            const BoundingBox2D& rect);
  glm::vec2 closestPointOnRect(const Circle2D& circle, const BoundingBox2D& rect);

  GameStateManager& gameState_;

  // Game boundaries - calculated from camera FOV=45°, distance=5.0, aspect≈1.78
  // Visible area: height ≈ 4.14, width ≈ 7.37, so bounds are:
  float WORLD_MIN_X = -2.0f;  // Left wall (was -3.5f)
  float WORLD_MAX_X = 2.0f;   // Right wall (was 3.5f)
  float WORLD_MIN_Y =  -2.0f;  // Kill plane (was -2.8f)
  float WORLD_MAX_Y = 2.0f;   // Top wall (was 2.0f)

  // Physics constants
  static constexpr float BOARD_SPEED_BOOST = 1.05f;
  static constexpr float BOARD_SPIN_FACTOR = 3.0f;
  static constexpr int POINTS_PER_BRICK = 10;
};

}  // namespace OpenGL::Game::Managers

#endif  // OPENGL_GAME_MANAGERS_COLLISION_MANAGER_H