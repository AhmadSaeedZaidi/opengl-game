#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"
#include "textures.h"

class Rectangle : public Shape {
 public:
  // coords = { Ax, Ay, Bx, By }
  // textureFile is now passed in
  Rectangle(const float coords[4], const char* textureFile);
  ~Rectangle() override;

  void init() override;
  void draw() override;

 private:
  // Vertex array / buffer objects
  GLuint VAO = 0, VBO = 0, EBO = 0;
  // The GL texture ID
  GLuint texID = 0;

  // Rectangle corner coordinates
  float Ax, Ay, Bx, By;
  const char* texPath;  // <- new
};

#endif  // RECTANGLE_H