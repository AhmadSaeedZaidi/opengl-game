#ifndef CUBOID
#define CUBOID

#include "shape.h"
#include "textures.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Cuboid : public Shape {
 public:
  // Create a cuboid with position, size, and up to 2 textures
  Cuboid(const glm::vec3& position, const glm::vec3& size, const char* atlas = nullptr, int x1 = 0,
         int y1 = 0, int w1 = 0, int h1 = 0, int x2 = 0, int y2 = 0, int w2 = 0, int h2 = 0);
  ~Cuboid() override;

  void init() override;
  void draw(GLuint ShaderID, float deltaTime) override;

  // Transform methods
  void setPosition(const glm::vec3& pos) {
    position_ = pos;
    updateModelMatrix();
  }
  void setRotation(const glm::vec3& rot) {
    rotation_ = rot;
    updateModelMatrix();
  }
  void setScale(const glm::vec3& scale) {
    scale_ = scale;
    updateModelMatrix();
  }

  // Animation
  void rotate(float angle, const glm::vec3& axis);
  void translate(const glm::vec3& offset);
  glm::mat4 getModelMatrix() const { return modelMatrix_; }

 protected:
  glm::mat4 modelMatrix_ = glm::mat4(1.0f);

 private:
  // Vertex array / buffer objects
  GLuint VAO = 0, VBO = 0, EBO = 0;

  // Textures (can have up to 2)
  GLuint sidesTexID = 0, capsTexID = 0;

  // Transform properties
  glm::vec3 position_;
  glm::vec3 rotation_ = glm::vec3(0.0f);
  glm::vec3 scale_;

  const char* atlasPath;
  int X1, Y1, W1, H1;  // Texture coordinates for first texture
  int X2, Y2, W2, H2;  // Texture coordinates for second texture

  void updateModelMatrix();
  void setupVertices();
};

#endif  // CUBOID_H