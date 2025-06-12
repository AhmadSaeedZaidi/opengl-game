#include "textures.h"
#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>
#include <cstring>

Textures::Textures(int width, int height, int nrChannels, const char* texturePath)
    : width(width), height(height), nrChannels(nrChannels), texturePath(texturePath) {
  init();
}
unsigned int Textures::getTexture() const { return texture; }

unsigned int Textures::loadTextureRegion(const char* filePath, int X, int Y, int W, int H) {
  int imgW, imgH, channels;
  unsigned char* data = stbi_load(filePath, &imgW, &imgH, &channels, 0);
  if (!data) throw std::runtime_error("Failed to load image");

  // Allocate buffer for the sub-image
  size_t rowSize = size_t(W) * channels;
  auto region = new unsigned char[size_t(H) * rowSize];

  // Copy each scanline
  for (int y = 0; y < H; ++y) {
    unsigned char* src = data + ((Y + y) * imgW + X) * channels;
    unsigned char* dst = region + size_t(y) * rowSize;
    std::memcpy(dst, src, rowSize);
  }
  stbi_image_free(data);

  // Upload to GL
  GLuint texID;
  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D, texID);
  GLenum fmt = (channels == 4 ? GL_RGBA : GL_RGB);
  glTexImage2D(GL_TEXTURE_2D, 0, fmt, W, H, 0, fmt, GL_UNSIGNED_BYTE, region);
  glGenerateMipmap(GL_TEXTURE_2D);

  delete[] region;
  return texID;
}
void Textures::init() {
  // Load the texture
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  // Set texture wrapping and filtering options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Load and generate the texture
  unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
  if (data) {
    GLenum fmt = (nrChannels == 4 ? GL_RGBA : GL_RGB);  // Dynamic format detection
    glTexImage2D(GL_TEXTURE_2D, 0, fmt, width, height, 0, fmt, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);
}
Textures::~Textures() {}
