#include "shaders.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

OpenGL::Core::Shader::Shader(const char* vertexPath, const char* fragmentPath) {
  // 1. Retrieve the vertex/fragment source code from filePath
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;

  // Ensure ifstream objects can throw exceptions
  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    // Open files
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);

    // Read file's buffer contents into streams
    std::stringstream vShaderStream, fShaderStream;
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    // Close file handlers
    vShaderFile.close();
    fShaderFile.close();

    // Convert stream into string
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  } catch (std::ifstream::failure& e) {
    std::cout << "ERROR::OpenGL::Core::Shader::FILE_NOT_SUCCESSFULLY_READ: " << e.what()
              << std::endl;
    throw std::runtime_error("Failed to read shader files");
  }

  const char* vShaderCode = vertexCode.c_str();
  const char* fShaderCode = fragmentCode.c_str();

  // 2. Compile shaders
  unsigned int vertex, fragment;
  int success;
  char infoLog[512];

  // Vertex Shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);

  // Print compile errors if any
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    std::cout << "ERROR::OpenGL::Core::Shader::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // Fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);

  // Print compile errors if any
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment, 512, NULL, infoLog);
    std::cout << "ERROR::OpenGL::Core::Shader::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // Shader Program
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);

  // Print linking errors if any
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID, 512, NULL, infoLog);
    std::cout << "ERROR::OpenGL::Core::Shader::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }

  // Delete the shaders as they're linked into our program now and no longer necessary
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void OpenGL::Core::Shader::use() { glUseProgram(ID); }

void OpenGL::Core::Shader::setBool(const std::string& name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
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

OpenGL::Core::Shader::~Shader() { glDeleteProgram(ID); }