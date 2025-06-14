#ifndef BOARD_3D
#define BOARD_3D

#include <iostream>
#include <cuboid.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

class Board3D : public Cuboid {
 public:
  Board3D();
  void draw(GLuint ShaderID, float deltaTime) override;

 private:
  float offsetX_ = 0.0f;          // current horizontal offset
  const float speed_ = 2.0f;      // world‐units per second
};

#endif