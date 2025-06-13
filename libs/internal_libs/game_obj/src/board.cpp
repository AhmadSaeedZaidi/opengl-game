#include "board.h"
#include <GLFW/glfw3.h>
#include <rectangle.h>

// 1) define your coords once
static constexpr float board_coords[4] = {
    -0.2f, -0.95f,  // bottom-left
    0.2f, -0.8f     // top-right
};

Board::Board() : Rectangle(board_coords, "textures/board.png") {}
void Board::draw(GLuint ShaderID, float deltaTime) {
  if (!ShaderID) {
    std::cerr << "ERROR: Shader ID is not set for Board::draw\n";
    return;
  }

  // 1) Input
  GLFWwindow* w = glfwGetCurrentContext();
  if (glfwGetKey(w, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS) {
    offsetX_ -= speed_ * deltaTime;
  }
  if (glfwGetKey(w, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS) {
    offsetX_ += speed_ * deltaTime;
  }

  // 2) Clamp to screen: [–1 + halfWidth_, +1 – halfWidth_]
  const float minX = -1.0f + halfWidth_;
  const float maxX = +1.0f - halfWidth_;
  offsetX_ = glm::clamp(offsetX_, minX, maxX);

  // 3) Build & upload model matrix
  trans = glm::translate(glm::mat4(1.0f), glm::vec3(offsetX_, 0.0f, 0.0f));

  // 4) Render
  Rectangle::draw(ShaderID, deltaTime);
}