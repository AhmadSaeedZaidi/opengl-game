#include "rectangle.h"
#include <glad/glad.h>

Rectangle::Rectangle(const float coords[4], const char* texturePath, int X, int Y, int W, int H)
    : Ax(coords[0]), Ay(coords[1]), Bx(coords[2]), By(coords[3]) {
    // load the sub‐texture and store it in our base Shape::texture
  unsigned int id = Textures::loadTextureRegion(texturePath, X, Y, W, H);
  setTexture(std::make_shared<Textures>(id));
}
Rectangle::~Rectangle() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

void Rectangle::setTexture(std::shared_ptr<Textures> tex) {
  texture = tex;
  hasTexture = true;
}
void Rectangle::init() {
  float verts[] = {
      // positions       // colors
      Ax, Ay, 0.0f, 1, 1, 1,  // top right
      Bx, By, 0.0f, 1, 1, 1,  // bottom left
      Ax, By, 0.0f, 1, 1, 1,  // bottom right
      Bx, Ay, 0.0f, 1, 1, 1   // top left
  };
  unsigned int idx[] = {0, 1, 2, 0, 2, 3};

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);
}

void Rectangle::draw() {
  if (hasTexture && texture) {
    glBindTexture(GL_TEXTURE_2D, texture->getTexture());
  }

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  if (hasTexture) {
    glBindTexture(GL_TEXTURE_2D, 0);  // Unbind texture
  }
}