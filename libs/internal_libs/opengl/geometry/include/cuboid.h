#ifndef CUBOID
#define CUBOID

#include <shape.h>
#include <textures.h>

namespace OpenGL::Geometry {
class Cuboid : public Shape {
 public:
  // Create a cuboid with position, size, and up to 2 textures
  Cuboid(const glm::vec3& position, const glm::vec3& size, const char* atlas = nullptr, int x1 = 0,
         int y1 = 0, int w1 = 0, int h1 = 0, int x2 = 0, int y2 = 0, int w2 = 0, int h2 = 0);
  ~Cuboid() override;

  void init() override;
  void draw(GLuint ShaderID, float deltaTime) override;

 protected:
  void updateModelMatrix() override;

 private:
  // Textures (can have up to 2)
  GLuint sidesTexID = 0, capsTexID = 0;

  // Texture atlas properties
  const char* atlasPath;
  int X1, Y1, W1, H1;  // Texture coordinates for first texture
  int X2, Y2, W2, H2;  // Texture coordinates for second texture

  void setupVertices();
};
}  // namespace OpenGL::Geometry
#endif  // CUBOID_H