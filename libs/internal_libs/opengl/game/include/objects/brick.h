#ifndef OPENGL_GAME_OBJECTS_BRICK_H
#define OPENGL_GAME_OBJECTS_BRICK_H

#include "cylinder.h"
#include <texture_atlas.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <utility>

// Forward declaration to avoid circular include
namespace OpenGL::Game::Managers {
struct BoundingBox2D;
}

namespace OpenGL::Game::Objects {

class Brick : public OpenGL::Geometry::Cylinder {
 public:
  // `sidesRegion` and `capsRegion` must exist in `atlas`
  // (e.g. "brick_sides", "brick_caps").
  Brick(const glm::vec3& position, OpenGL::Core::TextureAtlas& atlas, std::string sidesRegion,
        std::string capsRegion, const glm::vec3& color = glm::vec3(1.0f, 0.5f, 0.2f));
  ~Brick() = default;

  void draw(GLuint shaderID, float deltaTime, GLFWwindow* window) override;

  // Collision detection
  bool isDestroyed() const { return destroyed_; }
  void destroy() { destroyed_ = true; }

  // 2D Collision box (X/Y plane projection)
  Managers::BoundingBox2D getCollisionBox() const;  // Move implementation to .cpp

  // Utility methods for 3D bounds (add declarations)
  glm::vec3 getMin() const;
  glm::vec3 getMax() const;

 private:
  bool destroyed_ = false;  // Initialize here
  glm::vec3 color_;

  static constexpr float BRICK_WIDTH = 0.4f;
  static constexpr float BRICK_HEIGHT = 0.15f;
  static constexpr float BRICK_DEPTH = 0.2f;  // ADD THIS
};

}  // namespace OpenGL::Game::Objects

#endif  // OPENGL_GAME_OBJECTS_BRICK_H