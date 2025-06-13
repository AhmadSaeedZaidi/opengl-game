#include <iostream>
#include <rectangle.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

class Board : public Rectangle {
 public:
  Board();
  void draw(GLuint ShaderID, float deltaTime) override;

 private:
  float offsetX_ = 0.0f;          // current horizontal offset
  const float speed_ = 1.5f;      // world‐units per second
  const float halfWidth_ = 0.2f;  // half the width of the board
};