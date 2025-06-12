#include "textures.h"
#include <glad/glad.h>
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
unsigned int Textures::loadTextureRegion(const char* filePath, int x, int y, int w, int h) {
  int imgW, imgH, channels;
  unsigned char* data = stbi_load(filePath, &imgW, &imgH, &channels, 0);
  if (!data) {
    std::cerr << "ERROR: Failed to load '" << filePath << "'\n";
    return 0;
  }

  GLenum fmt = (channels == 4 ? GL_RGBA : GL_RGB);
  GLuint texID = 0;
  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D, texID);
  setupParams();

  // Tell GL how to skip rows/pixels so we pull only the sub‐region
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, imgW);
  glPixelStorei(GL_UNPACK_SKIP_PIXELS, x);
  glPixelStorei(GL_UNPACK_SKIP_ROWS, y);

  glTexImage2D(GL_TEXTURE_2D, 0, fmt, w, h, 0, fmt, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  // Restore defaults
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
  glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

  stbi_image_free(data);
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
