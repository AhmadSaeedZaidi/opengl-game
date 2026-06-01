#ifndef OPENGL_GEOMETRY_CYLINDER_H
#define OPENGL_GEOMETRY_CYLINDER_H

#include <shape.h>
#include <texture_atlas.h>
#include <glm/gtc/constants.hpp>
#include <string>

namespace OpenGL::Geometry {
class Cylinder : public Shape {
 public:
  // `sidesRegion` and `capsRegion` are looked up in `atlas` on the first
  // call to `init()`. Both must be present in the atlas.
  Cylinder(const glm::vec3& position, float radius, float height,
           OpenGL::Core::TextureAtlas& atlas, std::string sidesRegion, std::string capsRegion,
           int segments = 16);

  ~Cylinder() override;
  void init() override;
  void draw(GLuint ShaderID, float deltaTime, GLFWwindow* window) override;

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
  OpenGL::Core::TextureAtlas& atlas_;
  std::string sidesRegion_;
  std::string capsRegion_;
};
}  // namespace OpenGL::Geometry
#endif  // OPENGL_GEOMETRY_CYLINDER_H