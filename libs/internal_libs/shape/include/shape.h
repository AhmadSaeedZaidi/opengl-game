#ifndef SHAPE
#define SHAPE
#include <memory>
#include "textures.h"

class Shape {
 public:
  virtual ~Shape() = default;
  virtual void init() = 0;
  virtual void draw() = 0;

  virtual void setTexture(std::shared_ptr<Textures> tex);
  virtual unsigned int getTextureId() const;

 protected:
  std::shared_ptr<Textures> texture;
};

#endif