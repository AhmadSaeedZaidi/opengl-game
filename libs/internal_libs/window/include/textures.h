#ifndef TEXTURES
#define TEXTURES

class Textures {
 public:
  explicit Textures(unsigned int existingID)
      : texture(existingID), width(0), height(0), nrChannels(0), texturePath(nullptr) {}
  Textures(int width, int height, int nrChannels, const char* texturePath);
  ~Textures();
  static unsigned int loadTextureRegion(const char* filePath, int X, int Y, int W, int H);
  unsigned int getTexture() const;

 protected:
  virtual void init();
  const char* texturePath;
  int height, width, nrChannels;
  unsigned int texture;
};

#endif