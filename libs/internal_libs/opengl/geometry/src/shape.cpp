#include "shape.h"
#include "textures.h"

namespace OpenGL::Geometry {

void Shape::setTexture(std::shared_ptr<OpenGL::Core::Textures> tex) { texture = std::move(tex); }

unsigned int Shape::getTextureId() const { return texture ? texture->getTexture() : 0; }

void Shape::setPosition(const glm::vec3& pos) {
  position_ = pos;
  updateModelMatrix();
}

void Shape::setRotation(const glm::vec3& rot) {
  rotation_ = rot;
  updateModelMatrix();
}

void Shape::setScale(const glm::vec3& scale) {
  scale_ = scale;
  updateModelMatrix();
}

void Shape::rotate(float angle, const glm::vec3& axis) {
  if (axis.x != 0.0f) rotation_.x += angle;
  if (axis.y != 0.0f) rotation_.y += angle;
  if (axis.z != 0.0f) rotation_.z += angle;
  updateModelMatrix();
}

void Shape::translate(const glm::vec3& offset) {
  position_ += offset;
  updateModelMatrix();
}

void Shape::updateModelMatrix() {
  modelMatrix_ = glm::mat4(1.0f);
  modelMatrix_ = glm::translate(modelMatrix_, position_);

  // Apply rotations
  modelMatrix_ = glm::rotate(modelMatrix_, glm::radians(rotation_.x), glm::vec3(1.0f, 0.0f, 0.0f));
  modelMatrix_ = glm::rotate(modelMatrix_, glm::radians(rotation_.y), glm::vec3(0.0f, 1.0f, 0.0f));
  modelMatrix_ = glm::rotate(modelMatrix_, glm::radians(rotation_.z), glm::vec3(0.0f, 0.0f, 1.0f));

  modelMatrix_ = glm::scale(modelMatrix_, scale_);
}

}  // namespace OpenGL::Geometry