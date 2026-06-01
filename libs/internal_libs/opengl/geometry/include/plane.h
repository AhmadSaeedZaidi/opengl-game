#ifndef OPENGL_GEOMETRY_PLANE_H
#define OPENGL_GEOMETRY_PLANE_H

#include "shape.h"
#include "texture_atlas.h"
#include <string>

namespace OpenGL::Geometry {

class Plane : public Shape {
 public:
  // The texture is loaded as the named region from `atlas`.
  Plane(const glm::vec3& position, float width, float height, OpenGL::Core::TextureAtlas& atlas,
        std::string regionName);
  ~Plane();

  void init() override;
  void draw(GLuint shaderID, float deltaTime, GLFWwindow* window) override;

 private:
  float width_;
  float height_;
  OpenGL::Core::TextureAtlas& atlas_;
  std::string regionName_;
  GLuint textureID_;

  void generatePlane();
};

}  // namespace OpenGL::Geometry

#endif  // OPENGL_GEOMETRY_PLANE_H