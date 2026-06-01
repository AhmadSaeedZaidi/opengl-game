#ifndef OPENGL_CORE_TEXTURE_ATLAS_H
#define OPENGL_CORE_TEXTURE_ATLAS_H

#include <glad/glad.h>
#include <string>
#include <unordered_map>

namespace OpenGL::Core {

// A rectangular sub-region of an atlas image.
//
// `textureId()` is populated by TextureAtlas::getRegion() on first call,
// after which it is stable for the atlas's lifetime.
struct TextureRegion {
  int x = 0;
  int y = 0;
  int w = 0;
  int h = 0;

  // 0 means "not yet uploaded" (and is also what an upload failure returns).
  GLuint cachedId = 0;

  // Convenience accessor — same as reading `cachedId` directly.
  GLuint textureId() const { return cachedId; }
};

class TextureAtlas {
 public:
  // Loads `configPath` (a JSON file) and reads the source image path, the
  // named regions, and any full-image backgrounds. Throws std::runtime_error on:
  //   - file IO failure
  //   - JSON parse failure
  //   - missing 'atlas' / 'regions' keys
  //   - a region entry missing x/y/w/h
  explicit TextureAtlas(const std::string& configPath);

  // Returns the named region (either a packed sub-region or a full-image
  // background). Lazily uploads the texture to the GPU on first call.
  // Throws std::runtime_error if the name is not in the config. Non-const
  // because the first call performs a GPU upload that mutates the entry.
  const TextureRegion& getRegion(const std::string& name);

  // Path of the source atlas image (relative to the working dir at load time).
  const std::string& imagePath() const { return atlasImagePath_; }

 private:
  std::string atlasImagePath_;
  std::unordered_map<std::string, TextureRegion> regions_;
  // Names declared under the "backgrounds" array map to a full image path
  // rather than a sub-region of the main atlas. getRegion() checks this map
  // first and uploads the entire image on first reference.
  std::unordered_map<std::string, std::string> fullImagePaths_;
};

}  // namespace OpenGL::Core

#endif  // OPENGL_CORE_TEXTURE_ATLAS_H
