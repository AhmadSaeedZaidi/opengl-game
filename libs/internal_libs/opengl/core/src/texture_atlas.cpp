#include "texture_atlas.h"

#include <nlohmann/json.hpp>

#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <stb_image.h>
#include <vector>

#include "log.h"

namespace OpenGL::Core {

namespace {

// Set up sampler parameters for a freshly-uploaded subtexture. Project
// standard: REPEAT wrap, NEAREST filter.
void setupSubtextureParams() {
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

// stbi_load hands back rows top-down (origin = top-left of the source image),
// but OpenGL's UV space has its origin at the bottom-left of the texture.
// Without a flip, row 0 of the source image ends up at UV v=0 — i.e. the
// image's "top" is mapped to the bottom of any quad. The helper below copies
// `data` into a fresh row-reversed buffer so the upload is right-side up.
void copyRowsReversed(const unsigned char* src, unsigned char* dst, int imgW, int imgH,
                      int imgC) {
  const size_t rowBytes = static_cast<size_t>(imgW) * imgC;
  for (int row = 0; row < imgH; ++row) {
    std::memcpy(dst + static_cast<size_t>(row) * rowBytes,
                src + static_cast<size_t>(imgH - 1 - row) * rowBytes, rowBytes);
  }
}

// Normalize a rotation in degrees to one of {0, 90, 180, 270}.
// Returns 0 for any value that isn't a multiple of 90.
int normalizeRotation(int rotation) {
  int r = rotation % 360;
  if (r < 0) r += 360;
  if (r == 90 || r == 180 || r == 270) return r;
  return 0;
}

// Uploads the rect [rx, ry, rx+rw, ry+rh] of `imagePath` as a new GL texture,
// applying the stbi top-down axis-flip and an optional rotation in one pass.
// The resulting uploaded dimensions are written to `outW` / `outH` (these
// swap for 90°/270°). Returns 0 on any failure.
GLuint uploadRegionToGpu(const std::string& imagePath, int rx, int ry, int rw, int rh,
                         int rotation, int& outW, int& outH) {
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

  const int rot = normalizeRotation(rotation);
  if (rot == 90 || rot == 270) {
    outW = rh;
    outH = rw;
  } else {
    outW = rw;
    outH = rh;
  }

  // For each output pixel (outRow, outCol) compute the source pixel
  // (srcRow, srcCol) that maps to it. The mapping combines the stbi
  // top-down axis-flip (row reversal) with the requested rotation.
  std::vector<unsigned char> buf(static_cast<size_t>(outW) * outH * imgC);
  for (int outRow = 0; outRow < outH; ++outRow) {
    for (int outCol = 0; outCol < outW; ++outCol) {
      int srcRow, srcCol;
      switch (rot) {
        case 0:
          srcRow = rh - 1 - outRow;
          srcCol = outCol;
          break;
        case 90:
          srcRow = rh - 1 - outCol;
          srcCol = outRow;
          break;
        case 180:
          srcRow = rh - 1 - outRow;
          srcCol = rw - 1 - outCol;
          break;
        case 270:
        default:
          srcRow = outCol;
          srcCol = rw - 1 - outRow;
          break;
      }
      const unsigned char* src = data + ((ry + srcRow) * imgW + (rx + srcCol)) * imgC;
      unsigned char* dst = buf.data() + (outRow * outW + outCol) * imgC;
      std::memcpy(dst, src, imgC);
    }
  }

  const GLenum fmt = (imgC == 4 ? GL_RGBA : GL_RGB);
  GLuint texID = 0;
  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D, texID);
  setupSubtextureParams();
  glTexImage2D(GL_TEXTURE_2D, 0, fmt, outW, outH, 0, fmt, GL_UNSIGNED_BYTE, buf.data());
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);
  return texID;
}

// Uploads the full `imagePath` as a new GL texture. Records the image's
// dimensions into `outRegion` so callers can see the loaded size. Returns
// 0 on failure.
GLuint uploadFullImageToGpu(const std::string& imagePath, TextureRegion& outRegion) {
  int imgW = 0;
  int imgH = 0;
  int imgC = 0;
  unsigned char* data = stbi_load(imagePath.c_str(), &imgW, &imgH, &imgC, 0);
  if (!data) {
    std::cerr << "ERROR: Failed to load image '" << imagePath << "'\n";
    return 0;
  }

  std::vector<unsigned char> flipped(static_cast<size_t>(imgW) * imgH * imgC);
  copyRowsReversed(data, flipped.data(), imgW, imgH, imgC);

  const GLenum fmt = (imgC == 4 ? GL_RGBA : GL_RGB);
  GLuint texID = 0;
  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D, texID);
  setupSubtextureParams();
  glTexImage2D(GL_TEXTURE_2D, 0, fmt, imgW, imgH, 0, fmt, GL_UNSIGNED_BYTE, flipped.data());
  glGenerateMipmap(GL_TEXTURE_2D);

  outRegion.x = 0;
  outRegion.y = 0;
  outRegion.w = imgW;
  outRegion.h = imgH;

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
    // Optional "rotate" in degrees (0, 90, 180, 270). Other values are
    // rejected so the JSON doesn't silently fall back to no-rotation.
    r.rotation = 0;
    if (entry.contains("rotate")) {
      const auto& rot = entry["rotate"];
      if (!rot.is_number_integer()) {
        throw std::runtime_error("TextureAtlas: region '" + name +
                                 "' has a non-integer 'rotate' value");
      }
      const int deg = rot.get<int>();
      const int norm = ((deg % 360) + 360) % 360;
      if (norm != 0 && norm != 90 && norm != 180 && norm != 270) {
        throw std::runtime_error("TextureAtlas: region '" + name +
                                 "' has invalid rotation " + std::to_string(deg) +
                                 "° (must be 0, 90, 180, or 270)");
      }
      r.rotation = norm;
    }
    r.cachedId = 0;
    regions_.emplace(name, r);
  }

  // Optional "backgrounds" array: each entry is { name, path } for a full-image
  // background. The name must be unique across regions and backgrounds; the
  // path is loaded lazily on first getRegion() call.
  if (doc.contains("backgrounds")) {
    if (!doc["backgrounds"].is_array()) {
      throw std::runtime_error("TextureAtlas: 'backgrounds' must be an array");
    }
    for (const auto& bg : doc["backgrounds"]) {
      if (!bg.is_object() || !bg.contains("name") || !bg.contains("path") ||
          !bg["name"].is_string() || !bg["path"].is_string()) {
        throw std::runtime_error(
            "TextureAtlas: each background entry must be { name: string, path: string }");
      }
      const std::string name = bg["name"].get<std::string>();
      const std::string path = bg["path"].get<std::string>();
      if (regions_.count(name) || fullImagePaths_.count(name)) {
        throw std::runtime_error("TextureAtlas: duplicate texture name '" + name + "'");
      }
      TextureRegion r;  // dims filled in on first getRegion
      r.cachedId = 0;
      regions_.emplace(name, r);
      fullImagePaths_.emplace(name, path);
    }
  }

#if OPENGL_VERBOSE_LOG
  std::cout << "TextureAtlas: loaded " << regions_.size() << " textures from '" << configPath
            << "', atlas='" << atlasImagePath_ << "'\n";
#endif
}

const TextureRegion& TextureAtlas::getRegion(const std::string& name) {
  auto it = regions_.find(name);
  if (it == regions_.end()) {
    throw std::runtime_error("TextureAtlas: unknown region '" + name + "'");
  }

  // Lazy upload: each entry uploads at most once. Full-image backgrounds set
  // x/y/w/h from the loaded image dimensions; packed regions already have
  // those fields from the JSON config (and may have w/h updated below for
  // 90/270 rotations).
  if (it->second.cachedId == 0) {
    auto bgIt = fullImagePaths_.find(name);
    if (bgIt != fullImagePaths_.end()) {
      it->second.cachedId = uploadFullImageToGpu(bgIt->second, it->second);
    } else {
      int outW = 0, outH = 0;
      it->second.cachedId = uploadRegionToGpu(atlasImagePath_, it->second.x, it->second.y,
                                              it->second.w, it->second.h, it->second.rotation,
                                              outW, outH);
      it->second.w = outW;
      it->second.h = outH;
    }
  }
  return it->second;
}

}  // namespace OpenGL::Core
