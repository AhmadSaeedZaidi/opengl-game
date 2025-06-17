#ifndef SHAPE
#define SHAPE
#include <memory>
#include <textures.h>
#include <glad/glad.h>
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
  virtual void draw(GLuint shaderID, float deltaTime) = 0;

  // Texture methods
  virtual void setTexture(std::shared_ptr<OpenGL::Core::Textures> tex);
  virtual unsigned int getTextureId() const;

  // Common transform methods - DECLARATIONS ONLY
  virtual void setPosition(const glm::vec3& pos);
  virtual void setRotation(const glm::vec3& rot);
  virtual void setScale(const glm::vec3& scale);

  // Animation methods - DECLARATIONS ONLY
  virtual void rotate(float angle, const glm::vec3& axis);
  virtual void translate(const glm::vec3& offset);

  // Getters
  virtual glm::mat4 getModelMatrix() const { return modelMatrix_; }
  virtual glm::vec3 getPosition() const { return position_; }
  virtual glm::vec3 getRotation() const { return rotation_; }
  virtual glm::vec3 getScale() const { return scale_; }

 protected:
  std::shared_ptr<OpenGL::Core::Textures> texture;

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
#endif