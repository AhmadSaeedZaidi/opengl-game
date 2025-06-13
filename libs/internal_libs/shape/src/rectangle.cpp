#include "rectangle.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Rectangle::Rectangle(const float coords[4], const char* textureFile, int x, int y, int w, int h)
    : Ax(coords[0]),
      Ay(coords[1]),
      Bx(coords[2]),
      By(coords[3]),
      texPath(textureFile),
      x(x),
      y(y),
      w(w),
      h(h) {}

Rectangle::~Rectangle() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  if (texID) glDeleteTextures(1, &texID);
}

void Rectangle::init() {
  int w_, h_, ch;
  texID = Textures::loadTextureRegion(texPath, w_, h_, ch, x, y, w, h);
  if (!texID) {
    std::cerr << "Rectangle::init() failed to load '" << texPath << "'\n";
    return;
  }

  // Interpret: Ax=left, Ay=bottom, Bx=right, By=top
  float left = Ax, bottom = Ay;
  float right = Bx, top = By;

  float verts[] = {
      //  pos                color      UV
      left,  top,    0.0f, 1, 1, 1, 0.0f, 1.0f,  // top-left
      right, top,    0.0f, 1, 1, 1, 1.0f, 1.0f,  // top-right
      right, bottom, 0.0f, 1, 1, 1, 1.0f, 0.0f,  // bottom-right
      left,  bottom, 0.0f, 1, 1, 1, 0.0f, 0.0f   // bottom-left
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

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
}

void Rectangle::draw(GLuint shaderID, float deltaTime) {
  if (texID) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
  }

  unsigned int transformLoc = glGetUniformLocation(shaderID, "transform");
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);
  if (texID) glBindTexture(GL_TEXTURE_2D, 0);
}