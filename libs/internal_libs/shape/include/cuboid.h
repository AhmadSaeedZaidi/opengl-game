#ifndef CUBOID_H
#define CUBOID_H

#include "shape.h"

class Cuboid : public Shape {
 public:
  Cuboid(float width, float height, float depth);
  void draw(GLuint shaderProgram) override;

 private:
  float width_;
  float height_;
  float depth_;
};

#endif