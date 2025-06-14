#include "cuboid.h"
#include <iostream>

// Cube vertices with positions, normals, texture coordinates, and texture ID
// Format: x, y, z, nx, ny, nz, u, v, texID
static const float cubeVertices[] = {
    // Front face (z = 0.5) - SIDE texture (ID = 0)
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  // 0
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,   // 1
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,    // 2
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // 3

    // Back face (z = -0.5) - SIDE texture (ID = 0)
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f,  // 4
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,   // 5
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,    // 6
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f,   // 7

    // Left face (x = -0.5) - CAP texture (ID = 1) ← CHANGED: Now uses caps
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // 8
    -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,   // 9
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,    // 10
    -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // 11

    // Right face (x = 0.5) - CAP texture (ID = 1) ← CHANGED: Now uses caps
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // 12
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // 13
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,    // 14
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // 15

    // Top face (y = 0.5) - SIDE texture (ID = 0) ← CHANGED: Now uses sides
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // 16
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,   // 17
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,    // 18
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // 19

    // Bottom face (y = -0.5) - SIDE texture (ID = 0) ← CHANGED: Now uses sides
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // 20
    0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // 21
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,    // 22
    -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // 23
};

// Indices for the cube faces
static const unsigned int cubeIndices[] = {
    // Front face
    0, 1, 2, 2, 3, 0,
    // Back face
    4, 5, 6, 6, 7, 4,
    // Left face
    8, 9, 10, 10, 11, 8,
    // Right face
    12, 13, 14, 14, 15, 12,
    // Top face
    16, 17, 18, 18, 19, 16,
    // Bottom face
    20, 21, 22, 22, 23, 20};

Cuboid::Cuboid(const glm::vec3& position, const glm::vec3& size, const char* atlas, int x1, int y1,
               int w1, int h1, int x2, int y2, int w2, int h2)
    : position_(position),
      scale_(size),
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
  updateModelMatrix();
  std::cout << "Cuboid constructor called - position: (" << position.x << "," << position.y << ","
            << position.z << "), scale: (" << size.x << "," << size.y << "," << size.z << ")"
            << std::endl;
}

Cuboid::~Cuboid() {
  if (VAO) glDeleteVertexArrays(1, &VAO);
  if (VBO) glDeleteBuffers(1, &VBO);
  if (EBO) glDeleteBuffers(1, &EBO);
  if (sidesTexID) glDeleteTextures(1, &sidesTexID);
  if (capsTexID) glDeleteTextures(1, &capsTexID);
}

void Cuboid::init() {
  setupVertices();

  if (VAO == 0) {
    std::cout << "ERROR: VAO not created!" << std::endl;
  } else {
    std::cout << "VAO created: " << VAO << std::endl;
  }
  // Load textures if atlas is provided
  if (atlasPath) {
    int imgW, imgH, imgC;
    sidesTexID = Textures::loadTextureRegion(atlasPath, imgW, imgH, imgC, X1, Y1, W1, H1);
    capsTexID = Textures::loadTextureRegion(atlasPath, imgW, imgH, imgC, X2, Y2, W2, H2);
    if (!sidesTexID) {
      std::cerr << "Failed to load sides texture: " << atlasPath << std::endl;
    }
    if (!capsTexID) {
      std::cerr << "Failed to load caps texture: " << atlasPath << std::endl;
    }
  }
}

void Cuboid::setupVertices() {
  std::cout << "setupVertices() called" << std::endl;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  std::cout << "Generated VAO: " << VAO << ", VBO: " << VBO << ", EBO: " << EBO << std::endl;

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

  // Position attribute (location = 0)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Normal attribute (location = 1)
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Texture coordinate attribute (location = 2)
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // Texture ID attribute (location = 3) - tells shader which texture to use
  glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(8 * sizeof(float)));
  glEnableVertexAttribArray(3);

  glBindVertexArray(0);
  std::cout << "setupVertices() completed" << std::endl;
}

void Cuboid::updateModelMatrix() {
  modelMatrix_ = glm::mat4(1.0f);
  modelMatrix_ = glm::translate(modelMatrix_, position_);

  // Apply rotations
  modelMatrix_ = glm::rotate(modelMatrix_, glm::radians(rotation_.x), glm::vec3(1.0f, 0.0f, 0.0f));
  modelMatrix_ = glm::rotate(modelMatrix_, glm::radians(rotation_.y), glm::vec3(0.0f, 1.0f, 0.0f));
  modelMatrix_ = glm::rotate(modelMatrix_, glm::radians(rotation_.z), glm::vec3(0.0f, 0.0f, 1.0f));

  modelMatrix_ = glm::scale(modelMatrix_, scale_);
}

void Cuboid::rotate(float angle, const glm::vec3& axis) {
  if (axis.x != 0.0f) rotation_.x += angle;
  if (axis.y != 0.0f) rotation_.y += angle;
  if (axis.z != 0.0f) rotation_.z += angle;
  updateModelMatrix();
}

void Cuboid::translate(const glm::vec3& offset) {
  position_ += offset;
  updateModelMatrix();
}

void Cuboid::draw(GLuint shaderID, float deltaTime) {
  static int count = 0;
  if (count++ % 60 == 0) {
    std::cout << "Cuboid::draw() - VAO: " << VAO << ", VBO: " << VBO << ", EBO: " << EBO
              << std::endl;
  }

  if (VAO == 0) {
    std::cout << "ERROR: Trying to draw with VAO = 0!" << std::endl;
    return;
  }
  // Bind textures to different texture units
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

  // Set whether we have textures
  glUniform1i(glGetUniformLocation(shaderID, "hasSidesTexture"), sidesTexID != 0);
  glUniform1i(glGetUniformLocation(shaderID, "hasCapsTexture"), capsTexID != 0);

  // Draw the cube
  glBindVertexArray(VAO);

  if (count % 60 == 0) {
    std::cout << "Actually drawing triangles..." << std::endl;
  }

  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
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