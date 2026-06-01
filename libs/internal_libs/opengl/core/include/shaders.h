#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

namespace OpenGL::Core {

// Loads, compiles, and links a GLSL program from vertex and fragment source files.
class Shader {
 public:
  // The OpenGL program ID. Valid only after a successful constructor.
  unsigned int ID;

  // Throws std::runtime_error if either file cannot be read, if either shader
  // fails to compile, or if program linking fails.
  Shader(const char* vertexPath, const char* fragmentPath);
  ~Shader();

  // Binds the program for subsequent draw calls.
  void use();

  // Uniform setters. No-op if `name` does not exist in the linked program.
  void setBool(const std::string& name, bool value) const;
  void setInt(const std::string& name, int value) const;
  void setFloat(const std::string& name, float value) const;
  void setMat4(const std::string& name, const glm::mat4& mat) const;
  void setVec3(const std::string& name, const glm::vec3& value) const;
  void setVec3(const std::string& name, float x, float y, float z) const;
};

}  // namespace OpenGL::Core

#endif
