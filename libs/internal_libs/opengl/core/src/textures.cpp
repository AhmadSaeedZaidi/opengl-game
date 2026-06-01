#include "textures.h"
#include <glad/glad.h>
#include <stb_image.h>

#include <cstring>
#include <iostream>

static void setupParams() {
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

OpenGL::Core::Textures::Textures(int width, int height, int nrChannels, const char* texturePath)
    : width(width), height(height), nrChannels(nrChannels), texturePath(texturePath) {
  init();
}
unsigned int OpenGL::Core::Textures::getTexture() const { return texture; }

unsigned int OpenGL::Core::Textures::loadTexture(const char* filePath, int& outW, int& outH,
                                                 int& outChannels) {
  unsigned char* data = stbi_load(filePath, &outW, &outH, &outChannels, 0);
  if (!data) {
    std::cerr << "ERROR: Failed to load '" << filePath << "'\n";
    return 0;
  }
  GLenum fmt = (outChannels == 4 ? GL_RGBA : GL_RGB);
  GLuint texID;
  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D, texID);
  setupParams();
  glTexImage2D(GL_TEXTURE_2D, 0, fmt, outW, outH, 0, fmt, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);
  return texID;
}

unsigned int OpenGL::Core::Textures::loadTextureRegion(const char* filePath, int& outW, int& outH,
                                                       int& outChannels, int x, int y, int w,
                                                       int h) {
  unsigned char* data = stbi_load(filePath, &outW, &outH, &outChannels, 0);
  if (!data) {
    std::cerr << "ERROR: Failed to load '" << filePath << "'\n";
    return 0;
  }

  if (x < 0 || y < 0 || x + w > outW || y + h > outH) {
    std::cerr << "ERROR: Region out of bounds\n";
    stbi_image_free(data);
    return 0;
  }

  int C = outChannels;
  unsigned char* sub = new unsigned char[w * h * C];
  for (int row = 0; row < h; ++row) {
    unsigned char* dst = sub + row * w * C;
    unsigned char* src = data + ((y + row) * outW + x) * C;
    memcpy(dst, src, w * C);
  }

  GLenum fmt = (C == 4 ? GL_RGBA : GL_RGB);
  GLuint texID;
  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D, texID);
  setupParams();
  glTexImage2D(GL_TEXTURE_2D, 0, fmt, w, h, 0, fmt, GL_UNSIGNED_BYTE, sub);
  glGenerateMipmap(GL_TEXTURE_2D);

  delete[] sub;
  stbi_image_free(data);

  return texID;
}
void OpenGL::Core::Textures::init() {
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
  if (!data) {
    std::cerr << "ERROR: Failed to load texture '" << texturePath << "'\n";
    texture = 0;
    return;
  }
  GLenum fmt = (nrChannels == 4 ? GL_RGBA : GL_RGB);
  glTexImage2D(GL_TEXTURE_2D, 0, fmt, width, height, 0, fmt, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);
}
OpenGL::Core::Textures::~Textures() {
  if (texture) glDeleteTextures(1, &texture);
}
