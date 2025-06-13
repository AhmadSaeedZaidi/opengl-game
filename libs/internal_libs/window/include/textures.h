#ifndef TEXTURES
#define TEXTURES
#include <glad/glad.h>
#include <string>

class Textures {
 public:
  explicit Textures(unsigned int existingID)
      : texture(existingID), width(0), height(0), nrChannels(0), texturePath(nullptr) {}
  Textures(int width, int height, int nrChannels, const char* texturePath);
  ~Textures();

  // Load an entire image. Returns 0 on failure, or a GL texture ID.
  // outW/H/C will be filled in with the image’s width/height/channels.
  static unsigned int loadTexture(const char* filePath, int& outW, int& outH, int& outChannels);

  // Load just the sub‐rectangle [x,y,w,h] out of an image file.
  // Returns a new GL texture ID (0 on failure).
  static unsigned int loadTextureRegion(const char* filePath, int& outW, int& outH, int& outChannels, int x, int y, int w, int h);
  unsigned int getTexture() const;

 protected:
  virtual void init();
  const char* texturePath;
  int height, width, nrChannels;
  unsigned int texture;
};

#endif