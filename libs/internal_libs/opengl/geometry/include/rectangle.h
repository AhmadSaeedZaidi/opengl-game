#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <shape.h>
#include <textures.h>
#include <texture_atlas.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
namespace OpenGL::Geometry {
class Rectangle : public Shape {
 public:
  // coords = { Ax, Ay, Bx, By }. The texture comes from `atlas`[regionName].
  Rectangle(const float coords[4], OpenGL::Core::TextureAtlas& atlas, std::string regionName);
  ~Rectangle() override;

  void init() override;
  void draw(GLuint ShaderID, float deltaTime, GLFWwindow* window) override;

 protected:
  glm::mat4 trans = glm::mat4(1.0f);

 private:
  // Vertex array / buffer objects
  GLuint VAO = 0, VBO = 0, EBO = 0;
  // The GL texture ID
  GLuint texID = 0;

  // Rectangle corner coordinates
  float Ax, Ay, Bx, By;

  // Texture lookup info
  OpenGL::Core::TextureAtlas& atlas_;
  std::string regionName_;
};
}  // namespace OpenGL::Geometry
#endif  // RECTANGLE_H