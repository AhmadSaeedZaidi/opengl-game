#ifndef BOARD_3D
#define BOARD_3D

#include <iostream>
#include <cylinder.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

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

 private:
  const float speed_ = 2.0f;

  // Board dimensions for collision - make smaller
  static constexpr float BOARD_WIDTH = 0.8f;   // Was 1.0f - now smaller
  static constexpr float BOARD_HEIGHT = 0.2f;  // Was 0.3f - now thinner
};
}  // namespace OpenGL::Game::Objects
#endif  // BOARD_3D