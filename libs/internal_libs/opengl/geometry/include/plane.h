#ifndef OPENGL_GEOMETRY_PLANE_H
#define OPENGL_GEOMETRY_PLANE_H

#include "shape.h"

namespace OpenGL::Geometry {

class Plane : public Shape {
 public:
  Plane(const glm::vec3& position, float width, float height, const char* texturePath);
  ~Plane();

  void init() override;
  void draw(GLuint shaderID, float deltaTime) override;

 private:
  float width_;
  float height_;
  const char* texturePath_;
  GLuint textureID_;

  void generatePlane();
};

}  // namespace OpenGL::Geometry

#endif  // OPENGL_GEOMETRY_PLANE_H