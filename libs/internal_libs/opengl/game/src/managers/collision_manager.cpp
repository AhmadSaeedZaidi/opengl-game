#include "managers/collision_manager.h"
#include <algorithm>
#include <cmath>
#include <iostream>

OpenGL::Game::Managers::CollisionManager::CollisionManager(GameStateManager& gameState)
    : gameState_(gameState) {
  std::cout << "Collision Manager initialized" << std::endl;
}

void OpenGL::Game::Managers::CollisionManager::setBoundaries(float minX, float maxX, float minY,
                                                             float maxY) {
  WORLD_MIN_X = minX;
  WORLD_MAX_X = maxX;
  WORLD_MIN_Y = minY;
  WORLD_MAX_Y = maxY;
  std::cout << "Boundaries set: X[" << minX << "," << maxX << "] Y[" << minY << "," << maxY << "]"
            << std::endl;
}

void OpenGL::Game::Managers::CollisionManager::checkAllCollisions(
    Objects::Ball* ball, Objects::Board3D* board,
    std::vector<std::unique_ptr<Objects::Brick>>& bricks) {
  if (!ball || !gameState_.isPlaying()) return;

  // Check in order of priority
  checkBallBoundaries(ball);

  if (gameState_.isPlaying()) {  // Only continue if ball didn't die
    checkBallBrickCollisions(ball, bricks);
    checkBallBoardCollisions(ball, board);
  }
}

void OpenGL::Game::Managers::CollisionManager::checkBallBrickCollisions(
    Objects::Ball* ball, std::vector<std::unique_ptr<Objects::Brick>>& bricks) {
  if (!ball) return;

  Circle2D ballCircle = ball->getCollisionCircle();

  for (auto& brick : bricks) {
    if (!brick->isDestroyed()) {
      BoundingBox2D brickBox = brick->getCollisionBox();

      if (circleRect(ballCircle, brickBox)) {
        // Destroy brick
        brick->destroy();
        gameState_.addScore(POINTS_PER_BRICK);
        gameState_.incrementBricksDestroyed();

        // Calculate 2D bounce - no damping on brick hits either
        glm::vec2 newVelocity = calculateBounce(ballCircle, ball->getVelocity2D(), brickBox);
        ball->setBounceVelocity(newVelocity);  // Pure physics bounce

        std::cout << "Brick destroyed!" << std::endl;
        break;  // Only handle one collision per frame
      }
    }
  }
}

void OpenGL::Game::Managers::CollisionManager::checkBallBoardCollisions(Objects::Ball* ball,
                                                                        Objects::Board3D* board) {
  if (!ball || !board) return;

  Circle2D ballCircle = ball->getCollisionCircle();
  BoundingBox2D boardBox = board->getCollisionBox();

  if (circleRect(ballCircle, boardBox)) {
    // Boost the ball when it hits the paddle!
    glm::vec2 vel = ball->getVelocity2D();
    vel.y = std::abs(vel.y);  // Always bounce up

    // Add horizontal velocity based on where ball hits paddle
    float hitOffset = ballCircle.x - boardBox.x;
    vel.x += hitOffset * BOARD_SPIN_FACTOR;

    // Apply speed boost to ball
    float currentSpeed = glm::length(vel);
    vel = glm::normalize(vel) * (currentSpeed * BOARD_SPEED_BOOST);

    ball->setBounceVelocity(vel);

    // BOOST PADDLE SPEED!
    board->onBallHit();

    std::cout << "Ball hit paddle! Both speeds boosted!" << std::endl;
  }
}

void OpenGL::Game::Managers::CollisionManager::checkBallBoundaries(Objects::Ball* ball) {
  if (!ball) return;

  glm::vec3 ballPos = ball->getPosition();
  glm::vec2 ballVel = ball->getVelocity2D();
  float radius = ball->getRadius();
  bool bounced = false;

  // Left and right walls
  if (ballPos.x - radius <= WORLD_MIN_X) {
    ballPos.x = WORLD_MIN_X + radius;
    ballVel.x = std::abs(ballVel.x);  // Bounce right
    bounced = true;
  } else if (ballPos.x + radius >= WORLD_MAX_X) {
    ballPos.x = WORLD_MAX_X - radius;
    ballVel.x = -std::abs(ballVel.x);  // Bounce left
    bounced = true;
  }

  // Top wall
  if (ballPos.y + radius >= WORLD_MAX_Y) {
    ballPos.y = WORLD_MAX_Y - radius;
    ballVel.y = -std::abs(ballVel.y);  // Bounce down
    bounced = true;
  }

  // Kill plane (bottom)
  if (ballPos.y - radius <= WORLD_MIN_Y) {
    gameState_.loseLife();
    ball->resetToStart();
    std::cout << "Ball fell out of bounds!" << std::endl;
    return;
  }

  if (bounced) {
    ball->setPosition(ballPos);
    ball->setVelocity2D(ballVel);  // NO damping - perfect energy conservation!
  }
}

// ===== COLLISION DETECTION METHODS (moved from collision.cpp) =====

bool OpenGL::Game::Managers::CollisionManager::circleRect(const Circle2D& circle,
                                                          const BoundingBox2D& rect) {
  // Find closest point on rectangle to circle center
  float closestX = std::max(rect.left(), std::min(circle.x, rect.right()));
  float closestY = std::max(rect.bottom(), std::min(circle.y, rect.top()));

  // Calculate distance from circle center to closest point
  float deltaX = circle.x - closestX;
  float deltaY = circle.y - closestY;
  float distanceSquared = deltaX * deltaX + deltaY * deltaY;

  // Check if distance is less than radius
  return distanceSquared <= (circle.radius * circle.radius);
}

bool OpenGL::Game::Managers::CollisionManager::circleCircle(const Circle2D& c1,
                                                            const Circle2D& c2) {
  float deltaX = c1.x - c2.x;
  float deltaY = c1.y - c2.y;
  float distanceSquared = deltaX * deltaX + deltaY * deltaY;
  float radiusSum = c1.radius + c2.radius;

  return distanceSquared <= (radiusSum * radiusSum);
}

bool OpenGL::Game::Managers::CollisionManager::rectRect(const BoundingBox2D& r1,
                                                        const BoundingBox2D& r2) {
  return (r1.left() < r2.right() && r1.right() > r2.left() && r1.bottom() < r2.top() &&
          r1.top() > r2.bottom());
}

glm::vec2 OpenGL::Game::Managers::CollisionManager::calculateBounce(const Circle2D& circle,
                                                                    const glm::vec2& velocity,
                                                                    const BoundingBox2D& rect) {
  // Find closest point on rectangle
  glm::vec2 closest = closestPointOnRect(circle, rect);

  // Calculate normal from closest point to circle center
  glm::vec2 normal = glm::vec2(circle.x - closest.x, circle.y - closest.y);
  float length = sqrt(normal.x * normal.x + normal.y * normal.y);

  if (length > 0.001f) {
    normal.x /= length;
    normal.y /= length;
  } else {
    // Default to reversing velocity if we can't determine normal
    return glm::vec2(-velocity.x, -velocity.y);
  }

  // Reflect velocity: v' = v - 2(v·n)n
  float dotProduct = velocity.x * normal.x + velocity.y * normal.y;
  return glm::vec2(velocity.x - 2.0f * dotProduct * normal.x,
                   velocity.y - 2.0f * dotProduct * normal.y);
}

glm::vec2 OpenGL::Game::Managers::CollisionManager::closestPointOnRect(const Circle2D& circle,
                                                                       const BoundingBox2D& rect) {
  return glm::vec2(std::max(rect.left(), std::min(circle.x, rect.right())),
                   std::max(rect.bottom(), std::min(circle.y, rect.top())));
}