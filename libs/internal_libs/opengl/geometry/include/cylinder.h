// Create libs/internal_libs/opengl/geometry/include/cylinder.h
#ifndef CYLINDER_H
#define CYLINDER_H

#include <shape.h>
#include <textures.h>

namespace OpenGL::Geometry {
class Cylinder : public Shape {
 public:
  Cylinder(const glm::vec3& position = glm::vec3(0.0f), float radius = 1.0f, float height = 2.0f,
           const char* atlas = nullptr, int x1 = 0, int y1 = 0, int w1 = 0, int h1 = 0, int x2 = 0,
           int y2 = 0, int w2 = 0, int h2 = 0, int segments = 16);

  ~Cylinder() override;
  void init() override;
  void draw(GLuint ShaderID, float deltaTime) override;

 protected:
  void updateModelMatrix() override;

 private:
  float radius_, height_;
  int segments_;

  void generateCylinder();
  void generateSideVertices();
  void generateCapVertices();

  GLuint sidesTexID = 0, capsTexID = 0;

  // Texture atlas properties
  const char* atlasPath;
  int X1, Y1, W1, H1;  // Sides texture coordinates
  int X2, Y2, W2, H2;  // Caps texture coordinates
};
}  // namespace OpenGL::Geometry
#endif