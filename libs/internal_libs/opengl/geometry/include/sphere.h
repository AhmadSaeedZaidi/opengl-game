#ifndef OPENGL_GEOMETRY_SPHERE_H
#define OPENGL_GEOMETRY_SPHERE_H

#include "shape.h"
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace OpenGL::Geometry {

class Sphere : public Shape {
 public:
  Sphere(const glm::vec3& position, float radius, const char* atlas = nullptr, int x = 0,
         int y = 0, int w = 0, int h = 0, int latitudeSegments = 16, int longitudeSegments = 32);

  ~Sphere();

  void init() override;
  void draw(GLuint shaderID, float deltaTime) override;

 private:
  void generateSphere();
  void generateVertices();

  float radius_;
  int latitudeSegments_;   // Number of horizontal segments (rings)
  int longitudeSegments_;  // Number of vertical segments (slices)

  // Texture atlas info
  const char* atlasPath;
  int X, Y, W, H;
  GLuint textureID;

  std::vector<Vertex> vertices_;
  std::vector<unsigned int> indices_;
};

}  // namespace OpenGL::Geometry
#endif  // OPENGL_GEOMETRY_SPHERE_H