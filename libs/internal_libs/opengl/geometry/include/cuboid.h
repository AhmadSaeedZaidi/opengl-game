#ifndef OPENGL_GEOMETRY_CUBOID_H
#define OPENGL_GEOMETRY_CUBOID_H

#include <shape.h>
#include <textures.h>
#include <texture_atlas.h>
#include <string>

namespace OpenGL::Geometry {
class Cuboid : public Shape {
 public:
  // Create a cuboid with two atlas-driven textures. `sidesRegion` and
  // `capsRegion` must both be present in `atlas`.
  Cuboid(const glm::vec3& position, const glm::vec3& size, OpenGL::Core::TextureAtlas& atlas,
         std::string sidesRegion, std::string capsRegion);
  ~Cuboid() override;

  void init() override;
  void draw(GLuint ShaderID, float deltaTime, GLFWwindow* window) override;

 protected:
  void updateModelMatrix() override;

 private:
  // Textures (can have up to 2)
  GLuint sidesTexID = 0, capsTexID = 0;

  // Texture atlas properties
  OpenGL::Core::TextureAtlas& atlas_;
  std::string sidesRegion_;
  std::string capsRegion_;

  void setupVertices();
};
}  // namespace OpenGL::Geometry
#endif  // OPENGL_GEOMETRY_CUBOID_H