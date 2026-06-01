#include "shaders.h"

#include <glad/glad.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace {

// Reads a whole file into a std::string. Throws std::runtime_error on failure.
std::string readFile(const char* path) {
  std::ifstream stream(path);
  stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  std::stringstream buffer;
  buffer << stream.rdbuf();
  return buffer.str();
}

// Logs the GLSL compile log for `shader` to stderr, then deletes the shader
// object so we don't leak it on the failure path.
[[noreturn]] void throwCompileError(unsigned int shader, const char* stage) {
  char infoLog[1024];
  glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
  std::cerr << "ERROR::Shader::" << stage << "::COMPILATION_FAILED\n" << infoLog << std::endl;
  glDeleteShader(shader);
  throw std::runtime_error(std::string("Shader compilation failed: ") + stage);
}

}  // namespace

OpenGL::Core::Shader::Shader(const char* vertexPath, const char* fragmentPath) {
  std::string vertexCode;
  std::string fragmentCode;
  try {
    vertexCode = readFile(vertexPath);
    fragmentCode = readFile(fragmentPath);
  } catch (const std::ifstream::failure& e) {
    std::cerr << "ERROR::Shader::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    throw std::runtime_error("Failed to read shader files");
  } catch (const std::exception& e) {
    std::cerr << "ERROR::Shader::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    throw;
  }

  const char* vShaderCode = vertexCode.c_str();
  const char* fShaderCode = fragmentCode.c_str();

  int success;
  char infoLog[1024];

  unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, nullptr);
  glCompileShader(vertex);
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success) {
    throwCompileError(vertex, "VERTEX");
  }

  unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, nullptr);
  glCompileShader(fragment);
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if (!success) {
    glDeleteShader(vertex);
    throwCompileError(fragment, "FRAGMENT");
  }

  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);

  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID, sizeof(infoLog), nullptr, infoLog);
    std::cerr << "ERROR::Shader::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    glDetachShader(ID, vertex);
    glDetachShader(ID, fragment);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    glDeleteProgram(ID);
    ID = 0;
    throw std::runtime_error("Shader program linking failed");
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void OpenGL::Core::Shader::use() { glUseProgram(ID); }

void OpenGL::Core::Shader::setBool(const std::string& name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}

void OpenGL::Core::Shader::setInt(const std::string& name, int value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void OpenGL::Core::Shader::setFloat(const std::string& name, float value) const {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void OpenGL::Core::Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void OpenGL::Core::Shader::setVec3(const std::string& name, const glm::vec3& value) const {
  glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void OpenGL::Core::Shader::setVec3(const std::string& name, float x, float y, float z) const {
  glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

OpenGL::Core::Shader::~Shader() {
  if (ID) glDeleteProgram(ID);
}
