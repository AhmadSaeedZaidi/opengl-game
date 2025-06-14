#ifndef BOARD_3D
#define BOARD_3D

#include <iostream>
#include <cuboid.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

namespace OpenGL::Game::Objects {
class Board3D : public OpenGL::Geometry::Cuboid {  
 public:
  Board3D();
  void draw(GLuint ShaderID, float deltaTime) override;

 private:
  float offsetX_ = 0.0f;
  const float speed_ = 2.0f;
};
}  // namespace OpenGL::Game::Objects
#endif