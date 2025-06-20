#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <shape.h>
#include <textures.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace OpenGL::Geometry {
class Rectangle : public Shape {
 public:
  // coords = { Ax, Ay, Bx, By }
  // textureFile is now passed in
  Rectangle(const float coords[4], const char* textureFile, int x, int y, int w, int h);
  ~Rectangle() override;

  void init() override;
  void draw(GLuint ShaderID, float deltaTime) override;

 protected:
  glm::mat4 trans = glm::mat4(1.0f);

 private:
  // Vertex array / buffer objects
  GLuint VAO = 0, VBO = 0, EBO = 0;
  // The GL texture ID
  GLuint texID = 0;

  // Rectangle corner coordinates
  float Ax, Ay, Bx, By;
  // Texture coordinates in the atlas
  int x, y, w, h;
  const char* texPath;
};
}  // namespace OpenGL::Geometry
#endif  // RECTANGLE_H