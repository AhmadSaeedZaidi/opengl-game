#include "textures.h"
#include <glad/glad.h>
#include <stb_image_write.h>
#include <stb_image.h>
#include <iostream>

static void setupParams() {
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

Textures::Textures(int width, int height, int nrChannels, const char* texturePath)
    : width(width), height(height), nrChannels(nrChannels), texturePath(texturePath) {
  init();
}
unsigned int Textures::getTexture() const { return texture; }

// Load an entire image as a GL texture.
// Returns 0 on failure, or a valid texture ID.
unsigned int Textures::loadTexture(const char* filePath, int& outW, int& outH, int& outChannels) {
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

// Load a sub‐rectangle [x,y,w,h] out of an image file.
// Returns a new GL texture containing just that region.
unsigned int Textures::loadTextureRegion(const char* filePath, int& outW, int& outH,
                                         int& outChannels, int x, int y, int w, int h) {
  std::cout << "=== TEXTURE REGION DEBUG ===" << std::endl;
  std::cout << "Loading from: '" << filePath << "'" << std::endl;
  std::cout << "Requested region: x=" << x << ", y=" << y << ", w=" << w << ", h=" << h
            << std::endl;

  // 1) load the full image
  unsigned char* data = stbi_load(filePath, &outW, &outH, &outChannels, 0);
  if (!data) {
    std::cerr << "ERROR: Failed to load '" << filePath << "'\n";
    return 0;
  }
  std::cout << "✓ Image loaded: " << outW << "x" << outH << ", channels: " << outChannels
            << std::endl;

  // 2) bounds check
  if (x < 0 || y < 0 || x + w > outW || y + h > outH) {
    std::cerr << "ERROR: Region out of bounds\n";
    stbi_image_free(data);
    return 0;
  }

  // 3) extract sub-image
  int C = outChannels;
  unsigned char* sub = new unsigned char[w * h * C];
  for (int row = 0; row < h; ++row) {
    unsigned char* dst = sub + row * w * C;
    unsigned char* src = data + ((y + row) * outW + x) * C;
    memcpy(dst, src, w * C);
  }
  std::cout << "✓ Region extracted (" << w * h * C << " bytes)" << std::endl;

  // 4) DEBUG: Save extracted region to file
  std::string debugPath = "textures/debug_" + std::to_string(x) + "_" + std::to_string(y) + "_" +
                          std::to_string(w) + "x" + std::to_string(h) + ".png";
  if (stbi_write_png(debugPath.c_str(), w, h, C, sub, w * C)) {
    std::cout << "✓ Debug region saved to: " << debugPath << std::endl;
  } else {
    std::cout << "⚠ Failed to save debug region to: " << debugPath << std::endl;
  }

  // 4) upload as GL texture
  GLenum fmt = (C == 4 ? GL_RGBA : GL_RGB);
  GLuint texID;
  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D, texID);
  setupParams();
  glTexImage2D(GL_TEXTURE_2D, 0, fmt, w, h, 0, fmt, GL_UNSIGNED_BYTE, sub);
  glGenerateMipmap(GL_TEXTURE_2D);

  // 5) clean up
  delete[] sub;
  stbi_image_free(data);
  std::cout << "=========================" << std::endl;
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
