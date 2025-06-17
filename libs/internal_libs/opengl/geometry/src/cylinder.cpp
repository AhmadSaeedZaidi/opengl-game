#include "cylinder.h"
#include <iostream>
#include <cmath>

OpenGL::Geometry::Cylinder::Cylinder(const glm::vec3& position, float radius, float height,
                                     const char* atlas, int x1, int y1, int w1, int h1, int x2,
                                     int y2, int w2, int h2, int segments)
    : radius_(radius),
      height_(height),
      segments_(segments),
      atlasPath(atlas),
      X1(x1),
      Y1(y1),
      W1(w1),
      H1(h1),
      X2(x2),
      Y2(y2),
      W2(w2),
      H2(h2),
      sidesTexID(0),
      capsTexID(0) {
  position_ = position;
  scale_ = glm::vec3(1.0f);
  updateModelMatrix();
}

OpenGL::Geometry::Cylinder::~Cylinder() {
  if (VAO) glDeleteVertexArrays(1, &VAO);
  if (VBO) glDeleteBuffers(1, &VBO);
  if (EBO) glDeleteBuffers(1, &EBO);
  if (sidesTexID) glDeleteTextures(1, &sidesTexID);
  if (capsTexID) glDeleteTextures(1, &capsTexID);
}

void OpenGL::Geometry::Cylinder::init() {
  generateCylinder();

  if (VAO == 0) {
    std::cout << "ERROR: Cylinder VAO not created!" << std::endl;
  } else {
    std::cout << "Cylinder VAO created: " << VAO << " (segments: " << segments_ << ")"
              << std::endl;
  }

  // Load textures if atlas is provided
  if (atlasPath) {
    int imgW, imgH, imgC;
    sidesTexID =
        OpenGL::Core::Textures::loadTextureRegion(atlasPath, imgW, imgH, imgC, X1, Y1, W1, H1);
    capsTexID =
        OpenGL::Core::Textures::loadTextureRegion(atlasPath, imgW, imgH, imgC, X2, Y2, W2, H2);

    if (!sidesTexID) {
      std::cerr << "Failed to load cylinder sides texture" << std::endl;
    }
    if (!capsTexID) {
      std::cerr << "Failed to load cylinder caps texture" << std::endl;
    }
  }
}

void OpenGL::Geometry::Cylinder::generateCylinder() {
  vertices_.clear();
  indices_.clear();

  generateSideVertices();
  generateCapVertices();

  // Create OpenGL buffers
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

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
}

void OpenGL::Geometry::Cylinder::generateSideVertices() {
  // Generate vertices for cylinder sides oriented along X-axis
  // We need exactly 'segments' unique positions around the circumference
  for (int i = 0; i < segments_; ++i) {
    float angle = 2.0f * M_PI * i / segments_;
    float y = radius_ * cos(angle);
    float z = radius_ * sin(angle);

    glm::vec3 radialNormal = glm::normalize(glm::vec3(0.0f, y, z));

    // UV coordinate: wrap around the circumference
    float u = float(i) / float(segments_);

    // Left ring vertex (X = -height/2)
    Vertex leftVertex;
    leftVertex.position = glm::vec3(-height_ / 2.0f, y, z);
    leftVertex.normal = radialNormal;
    leftVertex.texCoord = glm::vec2(0.0f, -u);  // ROTATED: was (u, 0.0f), now (0.0f, u)
    vertices_.push_back(leftVertex);

    // Right ring vertex (X = +height/2)
    Vertex rightVertex;
    rightVertex.position = glm::vec3(height_ / 2.0f, y, z);
    rightVertex.normal = radialNormal;
    rightVertex.texCoord = glm::vec2(1.0f, -u);  // ROTATED: was (u, 1.0f), now (1.0f, u)
    vertices_.push_back(rightVertex);
  }

  // Generate triangles connecting the rings
  for (int i = 0; i < segments_; ++i) {
    unsigned int leftCurrent = i * 2;
    unsigned int rightCurrent = leftCurrent + 1;
    unsigned int leftNext = ((i + 1) % segments_) * 2;  // Wrap around using modulo
    unsigned int rightNext = leftNext + 1;

    // First triangle (counter-clockwise when viewed from outside)
    indices_.push_back(leftCurrent);
    indices_.push_back(rightCurrent);
    indices_.push_back(leftNext);

    // Second triangle
    indices_.push_back(leftNext);
    indices_.push_back(rightCurrent);
    indices_.push_back(rightNext);
  }
}

void OpenGL::Geometry::Cylinder::generateCapVertices() {
  unsigned int sideVertexCount = vertices_.size();

  // Left cap center (X = -height/2)
  Vertex leftCenter;
  leftCenter.position = glm::vec3(-height_ / 2.0f, 0.0f, 0.0f);
  leftCenter.normal = glm::vec3(-1.0f, 0.0f, 0.0f);  // Points left
  leftCenter.texCoord = glm::vec2(0.5f, 0.5f);       // Center of texture
  vertices_.push_back(leftCenter);
  unsigned int leftCenterIndex = vertices_.size() - 1;

  // Right cap center (X = +height/2)
  Vertex rightCenter;
  rightCenter.position = glm::vec3(height_ / 2.0f, 0.0f, 0.0f);
  rightCenter.normal = glm::vec3(1.0f, 0.0f, 0.0f);  // Points right
  rightCenter.texCoord = glm::vec2(0.5f, 0.5f);      // Center of texture
  vertices_.push_back(rightCenter);
  unsigned int rightCenterIndex = vertices_.size() - 1;

  // Left cap edge vertices (same positions as side vertices but different normals)
  for (int i = 0; i < segments_; ++i) {
    float angle = 2.0f * M_PI * i / segments_;
    float y = radius_ * cos(angle);
    float z = radius_ * sin(angle);

    Vertex vertex;
    vertex.position = glm::vec3(-height_ / 2.0f, y, z);
    vertex.normal = glm::vec3(-1.0f, 0.0f, 0.0f);  // Points left
    // Radial UV mapping for caps
    vertex.texCoord = glm::vec2((y / radius_ + 1.0f) * 0.5f, (z / radius_ + 1.0f) * 0.5f);
    vertices_.push_back(vertex);
  }

  // Right cap edge vertices
  for (int i = 0; i < segments_; ++i) {
    float angle = 2.0f * M_PI * i / segments_;
    float y = radius_ * cos(angle);
    float z = radius_ * sin(angle);

    Vertex vertex;
    vertex.position = glm::vec3(height_ / 2.0f, y, z);
    vertex.normal = glm::vec3(1.0f, 0.0f, 0.0f);  // Points right
    // Radial UV mapping for caps
    vertex.texCoord = glm::vec2((y / radius_ + 1.0f) * 0.5f, (z / radius_ + 1.0f) * 0.5f);
    vertices_.push_back(vertex);
  }

  // Left cap triangles (center to edge) - note the winding order
  unsigned int leftCapStart = sideVertexCount + 2;
  for (int i = 0; i < segments_; ++i) {
    int next = (i + 1) % segments_;
    indices_.push_back(leftCenterIndex);
    indices_.push_back(leftCapStart + next);  // Next vertex (counter-clockwise)
    indices_.push_back(leftCapStart + i);     // Current vertex
  }

  // Right cap triangles (center to edge) - note the winding order
  unsigned int rightCapStart = leftCapStart + segments_;
  for (int i = 0; i < segments_; ++i) {
    int next = (i + 1) % segments_;
    indices_.push_back(rightCenterIndex);
    indices_.push_back(rightCapStart + i);     // Current vertex
    indices_.push_back(rightCapStart + next);  // Next vertex (clockwise)
  }
}

void OpenGL::Geometry::Cylinder::updateModelMatrix() { Shape::updateModelMatrix(); }

void OpenGL::Geometry::Cylinder::draw(GLuint shaderID, float deltaTime) {
  if (VAO == 0) {
    std::cout << "ERROR: Trying to draw cylinder with VAO = 0!" << std::endl;
    return;
  }

  // Bind textures
  if (sidesTexID) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sidesTexID);
    glUniform1i(glGetUniformLocation(shaderID, "sidesTexture"), 0);
  }

  if (capsTexID) {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, capsTexID);
    glUniform1i(glGetUniformLocation(shaderID, "capsTexture"), 1);
  }

  // Send model matrix to shader
  unsigned int modelLoc = glGetUniformLocation(shaderID, "model");
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix_));

  // Set texture flags
  glUniform1i(glGetUniformLocation(shaderID, "hasSidesTexture"), sidesTexID != 0);
  glUniform1i(glGetUniformLocation(shaderID, "hasCapsTexture"), capsTexID != 0);

  // Draw the cylinder
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  // Unbind textures
  if (sidesTexID) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  if (capsTexID) {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}