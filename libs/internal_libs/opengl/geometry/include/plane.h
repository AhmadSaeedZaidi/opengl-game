#ifndef OPENGL_GEOMETRY_PLANE_H
#define OPENGL_GEOMETRY_PLANE_H

#include "shape.h"
#include "texture_atlas.h"
#include <string>

namespace OpenGL::Geometry {

class Plane : public Shape {
 public:
  // The texture is loaded as the named region from `atlas`. The optional
  // `uMin/uMax/vMin/vMax` crop the texture sampling to a sub-rectangle of
  // the loaded region — useful for fitting a 16:9 source image into a
  // square plane (defaults to the full 0..1 range).
  Plane(const glm::vec3& position, float width, float height, OpenGL::Core::TextureAtlas& atlas,
        std::string regionName, float uMin = 0.0f, float uMax = 1.0f, float vMin = 0.0f,
        float vMax = 1.0f);
  ~Plane();

  void init() override;
  void draw(GLuint shaderID, float deltaTime, GLFWwindow* window) override;

 private:
  float width_;
  float height_;
  float uMin_;
  float uMax_;
  float vMin_;
  float vMax_;
  OpenGL::Core::TextureAtlas& atlas_;
  std::string regionName_;
  GLuint textureID_;

  void generatePlane();
};

}  // namespace OpenGL::Geometry

#endif  // OPENGL_GEOMETRY_PLANE_H