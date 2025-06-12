#include "shape.h"
#include "textures.h"

void Shape::setTexture(std::shared_ptr<Textures> tex) { texture = std::move(tex); }

unsigned int Shape::getTextureId() const { return texture ? texture->getTexture() : 0; }