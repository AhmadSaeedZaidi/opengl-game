#ifndef RECTANGLE
#define RECTANGLE
#include <memory>
#include "shape.h"

class Rectangle : public Shape {
 public:
  // coords=[Ax,Ay,Bx,By]
  Rectangle(const float coords[4], const char* texturePath, int X, int Y, int W, int H);
  ~Rectangle();

  void init() override;
  void draw() override;

  void setTexture(std::shared_ptr<Textures> tex) override;

 private:
  unsigned int VAO = 0, VBO = 0, EBO = 0;
  float Ax, Ay, Bx, By;
  bool hasTexture = false;
};

#endif