#include "shape.h"
#include "textures.h"

void OpenGL::Geometry::Shape::setTexture(std::shared_ptr<OpenGL::Core::Textures> tex) {
  texture = std::move(tex);
}

unsigned int OpenGL::Geometry::Shape::getTextureId() const { return texture ? texture->getTexture() : 0; }