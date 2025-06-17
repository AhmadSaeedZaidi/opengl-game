#include "sphere.h"
#include "textures.h"
#include <iostream>
#include <cmath>

OpenGL::Geometry::Sphere::Sphere(const glm::vec3& position, float radius, const char* atlas, int x,
                                 int y, int w, int h, int latitudeSegments, int longitudeSegments)
    : radius_(radius),
      latitudeSegments_(latitudeSegments),
      longitudeSegments_(longitudeSegments),
      atlasPath(atlas),
      X(x),
      Y(y),
      W(w),
      H(h),
      textureID(0) {
  position_ = position;
  scale_ = glm::vec3(1.0f);
  updateModelMatrix();
}

OpenGL::Geometry::Sphere::~Sphere() {
  if (VAO) glDeleteVertexArrays(1, &VAO);
  if (VBO) glDeleteBuffers(1, &VBO);
  if (EBO) glDeleteBuffers(1, &EBO);
  if (textureID) glDeleteTextures(1, &textureID);
}

void OpenGL::Geometry::Sphere::init() {
  generateSphere();

  if (VAO == 0) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
  }

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), vertices_.data(),
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), indices_.data(),
               GL_STATIC_DRAW);

  // Position attribute (location = 0)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)offsetof(Vertex, position));
  glEnableVertexAttribArray(0);

  // Normal attribute (location = 1)
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
  glEnableVertexAttribArray(1);

  // Texture coordinate attribute (location = 2)
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)offsetof(Vertex, texCoord));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);

  // Load texture if atlas is provided
  if (atlasPath) {
    int w, h, channels;
    textureID = OpenGL::Core::Textures::loadTextureRegion(atlasPath, w, h, channels, X, Y, W, H);
    if (textureID == 0) {
      std::cerr << "Failed to load sphere texture from atlas: " << atlasPath << std::endl;
    }
  }
}

void OpenGL::Geometry::Sphere::generateSphere() {
  vertices_.clear();
  indices_.clear();
  generateVertices();
}

void OpenGL::Geometry::Sphere::generateVertices() {
  // Generate vertices
  for (int lat = 0; lat <= latitudeSegments_; ++lat) {
    float theta = lat * M_PI / latitudeSegments_;  // From 0 to PI
    float sinTheta = sin(theta);
    float cosTheta = cos(theta);

    for (int lon = 0; lon <= longitudeSegments_; ++lon) {
      float phi = lon * 2.0f * M_PI / longitudeSegments_;  // From 0 to 2*PI
      float sinPhi = sin(phi);
      float cosPhi = cos(phi);

      // Spherical coordinates to Cartesian
      float x = radius_ * sinTheta * cosPhi;
      float y = radius_ * cosTheta;
      float z = radius_ * sinTheta * sinPhi;

      Vertex vertex;
      vertex.position = glm::vec3(x, y, z);
      vertex.normal = glm::normalize(glm::vec3(x, y, z));  // Normal points outward from center

      // UV mapping
      float u = (float)lon / (float)longitudeSegments_;
      float v = (float)lat / (float)latitudeSegments_;
      vertex.texCoord = glm::vec2(u, v);

      vertices_.push_back(vertex);
    }
  }

  // Generate indices
  for (int lat = 0; lat < latitudeSegments_; ++lat) {
    for (int lon = 0; lon < longitudeSegments_; ++lon) {
      int current = lat * (longitudeSegments_ + 1) + lon;
      int next = current + longitudeSegments_ + 1;

      // First triangle
      indices_.push_back(current);
      indices_.push_back(next);
      indices_.push_back(current + 1);

      // Second triangle
      indices_.push_back(current + 1);
      indices_.push_back(next);
      indices_.push_back(next + 1);
    }
  }
}

void OpenGL::Geometry::Sphere::draw(GLuint shaderID, float deltaTime) {
  if (!shaderID) {
    std::cerr << "ERROR: Shader ID is not set for Sphere::draw\n";
    return;
  }

  // Use the shader and set uniforms
  glUseProgram(shaderID);

  // Set model matrix
  updateModelMatrix();
  glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE,
                     glm::value_ptr(getModelMatrix()));

  // Bind texture if available
  if (textureID) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(glGetUniformLocation(shaderID, "sphereTexture"), 0);
    glUniform1i(glGetUniformLocation(shaderID, "hasTexture"), 1);
  } else {
    glUniform1i(glGetUniformLocation(shaderID, "hasTexture"), 0);
  }

  // Draw the sphere
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}