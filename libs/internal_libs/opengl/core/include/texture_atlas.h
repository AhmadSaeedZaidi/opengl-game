#ifndef OPENGL_CORE_TEXTURE_ATLAS_H
#define OPENGL_CORE_TEXTURE_ATLAS_H

#include <glad/glad.h>
#include <string>
#include <unordered_map>

namespace OpenGL::Core {

// A rectangular sub-region of an atlas image.
//
// `textureId()` lazily uploads the region's pixels to a new GL texture on
// first call and caches the ID. The atlas itself is loaded once from disk at
// TextureAtlas construction; only the per-region subtexture upload is
// deferred to first use, so the initial startup cost is paid only for regions
// that are actually referenced.
struct TextureRegion {
  int x = 0;
  int y = 0;
  int w = 0;
  int h = 0;

  // 0 means "not yet uploaded" (and is also what an upload failure returns).
  // Populated by TextureAtlas::getRegion() and then stable for the atlas's
  // lifetime.
  mutable GLuint cachedId = 0;

  // Convenience accessor — same as reading `cachedId` directly.
  GLuint textureId() const { return cachedId; }
};

class TextureAtlas {
 public:
  // Loads `configPath` (a JSON file) and reads the source image path and the
  // named regions. Throws std::runtime_error on:
  //   - file IO failure
  //   - JSON parse failure
  //   - missing 'atlas' / 'regions' keys
  //   - a region entry missing x/y/w/h
  explicit TextureAtlas(const std::string& configPath);

  // Returns the named region. Throws std::runtime_error if the name is not in
  // the config.
  const TextureRegion& getRegion(const std::string& name) const;

  // Path of the source atlas image (relative to the working dir at load time).
  const std::string& imagePath() const { return atlasImagePath_; }

 private:
  std::string atlasImagePath_;
  std::unordered_map<std::string, TextureRegion> regions_;
};

}  // namespace OpenGL::Core

#endif  // OPENGL_CORE_TEXTURE_ATLAS_H
