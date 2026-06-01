#include "texture_atlas.h"

#include <nlohmann/json.hpp>

#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <stb_image.h>

#include "log.h"

namespace OpenGL::Core {

namespace {

// Set up sampler parameters for a freshly-uploaded subtexture. Matches the
// setup used elsewhere in `Textures` (REPEAT wrap, NEAREST filter).
void setupSubtextureParams() {
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

// Uploads the rect [rx, ry, rx+rw, ry+rh] of `imagePath` as a new GL texture.
// Returns 0 on any failure.
GLuint uploadRegionToGpu(const std::string& imagePath, int rx, int ry, int rw, int rh) {
  int imgW = 0;
  int imgH = 0;
  int imgC = 0;
  unsigned char* data = stbi_load(imagePath.c_str(), &imgW, &imgH, &imgC, 0);
  if (!data) {
    std::cerr << "ERROR: Failed to load atlas image '" << imagePath << "'\n";
    return 0;
  }

  if (rx < 0 || ry < 0 || rx + rw > imgW || ry + rh > imgH) {
    std::cerr << "ERROR: Region out of bounds for '" << imagePath << "' (region=" << rx << ","
              << ry << "," << rw << "," << rh << " image=" << imgW << "x" << imgH << ")\n";
    stbi_image_free(data);
    return 0;
  }

  unsigned char* sub = new unsigned char[rw * rh * imgC];
  for (int row = 0; row < rh; ++row) {
    unsigned char* dst = sub + row * rw * imgC;
    unsigned char* src = data + ((ry + row) * imgW + rx) * imgC;
    std::memcpy(dst, src, rw * imgC);
  }

  const GLenum fmt = (imgC == 4 ? GL_RGBA : GL_RGB);
  GLuint texID = 0;
  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D, texID);
  setupSubtextureParams();
  glTexImage2D(GL_TEXTURE_2D, 0, fmt, rw, rh, 0, fmt, GL_UNSIGNED_BYTE, sub);
  glGenerateMipmap(GL_TEXTURE_2D);

  delete[] sub;
  stbi_image_free(data);
  return texID;
}

}  // namespace

TextureAtlas::TextureAtlas(const std::string& configPath) {
  std::ifstream in(configPath);
  if (!in.is_open()) {
    throw std::runtime_error("TextureAtlas: cannot open config file '" + configPath + "'");
  }

  nlohmann::json doc;
  try {
    in >> doc;
  } catch (const std::exception& e) {
    throw std::runtime_error(std::string("TextureAtlas: JSON parse error: ") + e.what());
  }

  if (!doc.contains("atlas") || !doc["atlas"].is_string()) {
    throw std::runtime_error("TextureAtlas: config is missing a string 'atlas' field");
  }
  atlasImagePath_ = doc["atlas"].get<std::string>();

  if (!doc.contains("regions") || !doc["regions"].is_object()) {
    throw std::runtime_error("TextureAtlas: config is missing an object 'regions' field");
  }

  for (const auto& [name, entry] : doc["regions"].items()) {
    if (!entry.is_object() || !entry.contains("x") || !entry.contains("y") ||
        !entry.contains("w") || !entry.contains("h")) {
      throw std::runtime_error("TextureAtlas: region '" + name +
                               "' must be an object with x, y, w, h");
    }
    TextureRegion r;
    r.x = entry["x"].get<int>();
    r.y = entry["y"].get<int>();
    r.w = entry["w"].get<int>();
    r.h = entry["h"].get<int>();
    r.cachedId = 0;
    regions_.emplace(name, r);
  }

#if OPENGL_VERBOSE_LOG
  std::cout << "TextureAtlas: loaded " << regions_.size() << " regions from '" << configPath
            << "', atlas='" << atlasImagePath_ << "'\n";
#endif
}

const TextureRegion& TextureAtlas::getRegion(const std::string& name) const {
  auto it = regions_.find(name);
  if (it == regions_.end()) {
    throw std::runtime_error("TextureAtlas: unknown region '" + name + "'");
  }

  // `cachedId` is `mutable`, so this assignment is legal even on a const map
  // entry. The lazy upload is what makes `getRegion` non-const semantically
  // even though we keep the public API const.
  if (it->second.cachedId == 0) {
    it->second.cachedId = uploadRegionToGpu(atlasImagePath_, it->second.x, it->second.y,
                                            it->second.w, it->second.h);
  }
  return it->second;
}

}  // namespace OpenGL::Core
