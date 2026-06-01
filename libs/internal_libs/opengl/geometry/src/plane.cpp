#include "plane.h"
#include "log.h"
#include <iostream>

OpenGL::Geometry::Plane::Plane(const glm::vec3& position, float width, float height,
                                 OpenGL::Core::TextureAtlas& atlas, std::string regionName,
                                 float uMin, float uMax, float vMin, float vMax)
    : width_(width),
      height_(height),
      uMin_(uMin),
      uMax_(uMax),
      vMin_(vMin),
      vMax_(vMax),
      atlas_(atlas),
      regionName_(std::move(regionName)),
      textureID_(0) {
  position_ = position;
  scale_ = glm::vec3(1.0f);
  updateModelMatrix();
}

OpenGL::Geometry::Plane::~Plane() {
  if (VAO) glDeleteVertexArrays(1, &VAO);
  if (VBO) glDeleteBuffers(1, &VBO);
  if (EBO) glDeleteBuffers(1, &EBO);
}

void OpenGL::Geometry::Plane::init() {
  generatePlane();

  // Generate OpenGL objects
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  // Upload vertex data
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), vertices_.data(), GL_STATIC_DRAW);

  // Upload index data
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), indices_.data(), GL_STATIC_DRAW);

  // Set vertex attributes
  // Position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  glEnableVertexAttribArray(0);
  // Normal
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
  glEnableVertexAttribArray(1);
  // Texture coordinates
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);

  if (!regionName_.empty()) {
    const OpenGL::Core::TextureRegion& region = atlas_.getRegion(regionName_);
    textureID_ = region.textureId();
    if (textureID_ == 0) {
      std::cerr << "Plane::init() failed to load region '" << regionName_ << "'\n";
    }
  }

#if OPENGL_VERBOSE_LOG
  std::cout << "Plane initialized" << std::endl;
#endif
}

void OpenGL::Geometry::Plane::generatePlane() {
  vertices_.clear();
  indices_.clear();

  float halfWidth = width_ * 0.5f;
  float halfHeight = height_ * 0.5f;

  // Create 4 vertices for a quad (facing towards +Z, i.e., towards camera)
  Vertex v1, v2, v3, v4;

  // Bottom-left
  v1.position = glm::vec3(-halfWidth, -halfHeight, 0.0f);
  v1.normal = glm::vec3(0.0f, 0.0f, 1.0f);  // Normal pointing towards camera
  v1.texCoord = glm::vec2(uMin_, vMin_);

  // Bottom-right
  v2.position = glm::vec3(halfWidth, -halfHeight, 0.0f);
  v2.normal = glm::vec3(0.0f, 0.0f, 1.0f);
  v2.texCoord = glm::vec2(uMax_, vMin_);

  // Top-right
  v3.position = glm::vec3(halfWidth, halfHeight, 0.0f);
  v3.normal = glm::vec3(0.0f, 0.0f, 1.0f);
  v3.texCoord = glm::vec2(uMax_, vMax_);

  // Top-left
  v4.position = glm::vec3(-halfWidth, halfHeight, 0.0f);
  v4.normal = glm::vec3(0.0f, 0.0f, 1.0f);
  v4.texCoord = glm::vec2(uMin_, vMax_);

  vertices_ = {v1, v2, v3, v4};

  // Two triangles forming a quad
  indices_ = {
    0, 1, 2,  // First triangle
    2, 3, 0   // Second triangle
  };
}

void OpenGL::Geometry::Plane::draw(GLuint shaderID, float deltaTime, GLFWwindow* /*window*/) {
  if (!shaderID) {
    std::cerr << "ERROR: Shader ID is not set for Plane::draw\n";
    return;
  }

  glUseProgram(shaderID);

  // Set model matrix
  updateModelMatrix();
  glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, glm::value_ptr(getModelMatrix()));

  // Bind texture to slot 0 (sidesTexture)
  if (textureID_) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID_);
    glUniform1i(glGetUniformLocation(shaderID, "sidesTexture"), 0);
    glUniform1i(glGetUniformLocation(shaderID, "hasSidesTexture"), 1);
  } else {
    glUniform1i(glGetUniformLocation(shaderID, "hasSidesTexture"), 0);
  }

  // Draw the plane
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}