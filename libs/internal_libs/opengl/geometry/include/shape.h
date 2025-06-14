#ifndef SHAPE
#define SHAPE
#include <memory>
#include <textures.h>

namespace OpenGL::Geometry {
class Shape {
 public:
  virtual ~Shape() = default;
  virtual void init() = 0;
  virtual void draw(GLuint shaderID, float deltaTime) = 0;

  virtual void setTexture(std::shared_ptr<OpenGL::Core::Textures> tex);
  virtual unsigned int getTextureId() const;

 protected:
  std::shared_ptr<OpenGL::Core::Textures> texture;
};
}  // namespace OpenGL::Geometry
#endif