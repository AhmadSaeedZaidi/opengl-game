#ifndef OPENGL_GEOMETRY_SPHERE_H
#define OPENGL_GEOMETRY_SPHERE_H

#include "shape.h"
#include "texture_atlas.h"
#include <glm/gtc/constants.hpp>
#include <string>
#include <vector>

namespace OpenGL::Geometry {

class Sphere : public Shape {
 public:
  // The texture is looked up by `regionName` in the supplied `atlas` on the
  // first call to `init()`.
  Sphere(const glm::vec3& position, float radius, OpenGL::Core::TextureAtlas& atlas,
         std::string regionName, int latitudeSegments = 16, int longitudeSegments = 32);

  ~Sphere();

  void init() override;
  void draw(GLuint shaderID, float deltaTime, GLFWwindow* window) override;

 private:
  void generateSphere();
  void generateVertices();

  float radius_;
  int latitudeSegments_;   // Number of horizontal segments (rings)
  int longitudeSegments_;  // Number of vertical segments (slices)

  // Texture atlas info
  OpenGL::Core::TextureAtlas& atlas_;
  std::string regionName_;
  GLuint textureID;

  std::vector<Vertex> vertices_;
  std::vector<unsigned int> indices_;
};

}  // namespace OpenGL::Geometry
#endif  // OPENGL_GEOMETRY_SPHERE_H