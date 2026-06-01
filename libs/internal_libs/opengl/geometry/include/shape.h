#ifndef OPENGL_GEOMETRY_SHAPE_H
#define OPENGL_GEOMETRY_SHAPE_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

namespace OpenGL::Geometry {

// Common vertex structure for all shapes
struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoord;
};

class Shape {
 public:
  virtual ~Shape() = default;
  virtual void init() = 0;
  // `window` is passed through so subclasses that need GLFW input
  // (e.g. Board3D's keyboard polling) can read it without going through
  // the global `glfwGetCurrentContext()`. Shapes that don't need it can
  // simply ignore the argument.
  virtual void draw(GLuint shaderID, float deltaTime, GLFWwindow* window) = 0;

  // Common transform methods - DECLARATIONS ONLY
  virtual void setPosition(const glm::vec3& pos);
  virtual void setRotation(const glm::vec3& rot);
  virtual void setScale(const glm::vec3& scale);

  // Animation methods - DECLARATIONS ONLY
  virtual void rotate(float angle, const glm::vec3& axis);
  virtual void translate(const glm::vec3& offset);

  // Getters
  glm::mat4 getModelMatrix() const { return modelMatrix_; }
  glm::vec3 getPosition() const { return position_; }
  glm::vec3 getRotation() const { return rotation_; }
  glm::vec3 getScale() const { return scale_; }

 protected:
  // Common transform properties
  glm::mat4 modelMatrix_ = glm::mat4(1.0f);
  glm::vec3 position_ = glm::vec3(0.0f);
  glm::vec3 rotation_ = glm::vec3(0.0f);
  glm::vec3 scale_ = glm::vec3(1.0f);

  // Common OpenGL objects
  GLuint VAO = 0, VBO = 0, EBO = 0;
  std::vector<Vertex> vertices_;
  std::vector<unsigned int> indices_;

  // Pure virtual method for updating model matrix
  virtual void updateModelMatrix();
};
}  // namespace OpenGL::Geometry
#endif  // OPENGL_GEOMETRY_SHAPE_H