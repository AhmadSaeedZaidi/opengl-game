#include "game.h"
#include <memory>
#include <vector>
#include <iostream>

OpenGL::Game::Game::Game(int width, int height, const char* title, const char* vertexShader,
                         const char* fragmentShader)
    : OpenGL::Core::WindowApp(width, height, title),
      shader_(vertexShader, fragmentShader),
      camera_(5.0f),  // Start camera 5 units away
      gameState_(),
      collisionManager_(gameState_),
      hud_(gameState_) {}

OpenGL::Game::Game::~Game() = default;

void OpenGL::Game::Game::addShape(std::unique_ptr<OpenGL::Geometry::Shape> shape) {
  shapes_.emplace_back(std::move(shape));
}

void OpenGL::Game::Game::addBrick(std::unique_ptr<Objects::Brick> brick) {
  bricks_.emplace_back(std::move(brick));
}

void OpenGL::Game::Game::setBall(std::unique_ptr<Objects::Ball> ball) { ball_ = std::move(ball); }

void OpenGL::Game::Game::setBoard(std::unique_ptr<Objects::Board3D> board) {
  board_ = std::move(board);
}

void OpenGL::Game::Game::init() {
  OpenGL::Core::WindowApp::init();

  // Enable depth testing for 3D
  glEnable(GL_DEPTH_TEST);

  // Initialize shader
  shader_.use();

  // Set texture uniforms
  shader_.setInt("sidesTexture", 0);
  shader_.setInt("capsTexture", 1);
  shader_.setInt("sphereTexture", 2);
  shader_.setBool("hasSidesTexture", true);
  shader_.setBool("hasCapsTexture", true);
  shader_.setBool("hasTexture", true);

  // Initialize all shapes
  std::cout << "Game::init() - Initializing " << shapes_.size() << " shapes" << std::endl;
  for (size_t i = 0; i < shapes_.size(); ++i) {
    shapes_[i]->init();
  }

  // Initialize game objects
  if (ball_) ball_->init();
  if (board_) board_->init();
  for (auto& brick : bricks_) {
    brick->init();
  }

  // Create default brick layout if none exist
  if (bricks_.empty()) {
    createBrickLayout();
  }

  // Set up collision boundaries to match camera's actual view
  collisionManager_.setBoundaries(-2.0f, 2.0f, -2.0f, 2.0f);

  // Initialize HUD
  hud_.init();

  // Initialize game state
  gameState_.setRemainingBricks(bricks_.size());
  gameState_.setState(Managers::GameState::PLAYING);

  setupUniforms();
}

void OpenGL::Game::Game::update(float deltaTime) {
  // Process camera input
  camera_.processMouseInput(window);

  // Process game-specific input
  processGameInput();

  // Update game logic only if playing
  if (gameState_.isPlaying()) {
    updateGameLogic(deltaTime);
  }

  // Update HUD
  hud_.update(deltaTime);
}

void OpenGL::Game::Game::updateGameLogic(float deltaTime) {
  // Update ball physics
  if (ball_) {
    ball_->update(deltaTime);

    // Use collision manager for all collision detection
    collisionManager_.checkAllCollisions(ball_.get(), board_.get(), bricks_);
  }

  // Update remaining bricks count
  int remainingBricks = 0;
  for (const auto& brick : bricks_) {
    if (!brick->isDestroyed()) {
      remainingBricks++;
    }
  }
  gameState_.setRemainingBricks(remainingBricks);

  // Check win condition
  if (allBricksDestroyed()) {
    gameState_.setState(Managers::GameState::WON);
  }

  // Check lose condition (ball fell off screen)
  if (ball_ && ball_->getPosition().y < -4.0f) {
    gameState_.loseLife();
    if (gameState_.getLives() <= 0) {
      gameState_.setState(Managers::GameState::GAME_OVER);
    } else {
      // Reset ball position
      ball_->resetToStart();
    }
  }
}

void OpenGL::Game::Game::render(float deltaTime) {
  // Clear with depth buffer
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shader_.use();
  setupUniforms();

  renderGameObjects(deltaTime);

  // Render HUD
  hud_.render();
}

void OpenGL::Game::Game::renderGameObjects(float deltaTime) {
  // Render regular shapes
  for (size_t i = 0; i < shapes_.size(); ++i) {
    shapes_[i]->draw(shader_.ID, deltaTime);
  }

  // Render game objects
  if (ball_) ball_->draw(shader_.ID, deltaTime);
  if (board_) board_->draw(shader_.ID, deltaTime);
  for (auto& brick : bricks_) {
    if (!brick->isDestroyed()) {
      brick->draw(shader_.ID, deltaTime);
    }
  }
}

void OpenGL::Game::Game::processInput() {
  // Base class escape key handling
  OpenGL::Core::WindowApp::processInput();
}

void OpenGL::Game::Game::processGameInput() {
  // Static variables need to be at function scope, not inside if-blocks
  static bool spacePressed = false;
  static bool pPressed = false;

  // Reset game with Space key
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    if (!spacePressed) {
      resetGame();
      spacePressed = true;
    }
  } else {
    spacePressed = false;
  }

  // Pause/unpause with P key
  if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
    if (!pPressed) {
      if (gameState_.isPlaying()) {
        gameState_.setState(Managers::GameState::PAUSED);
        std::cout << "Game PAUSED" << std::endl;
      } else if (gameState_.getCurrentState() == Managers::GameState::PAUSED) {
        gameState_.setState(Managers::GameState::PLAYING);
        std::cout << "Game RESUMED" << std::endl;
      }
      pPressed = true;
    }
  } else {
    pPressed = false;
  }
}

void OpenGL::Game::Game::setupUniforms() {
  // Set view and projection matrices
  float aspectRatio = (float)width / (float)height;
  glm::mat4 view = camera_.getViewMatrix();
  glm::mat4 projection = camera_.getProjectionMatrix(aspectRatio);
  shader_.setMat4("view", view);
  shader_.setMat4("projection", projection);
}

void OpenGL::Game::Game::createBrickLayout() {
  // Create a simple brick layout
  const int rows = 4;
  const int cols = 8;
  const float brickSpacing = 0.5f;
  const float startX = -(cols - 1) * brickSpacing * 0.5f;
  const float startY = 1.0f;
  const float startZ = 0.0f;

  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
      glm::vec3 position(startX + col * brickSpacing, startY + row * 0.3f, startZ);

      // Different colors for different rows
      glm::vec3 color;
      switch (row) {
        case 0:
          color = glm::vec3(1.0f, 0.0f, 0.0f);
          break;  // Red
        case 1:
          color = glm::vec3(1.0f, 0.5f, 0.0f);
          break;  // Orange
        case 2:
          color = glm::vec3(1.0f, 1.0f, 0.0f);
          break;  // Yellow
        case 3:
          color = glm::vec3(0.0f, 1.0f, 0.0f);
          break;  // Green
      }

      auto brick = std::make_unique<Objects::Brick>(position, color);
      brick->init();
      bricks_.emplace_back(std::move(brick));
    }
  }

  std::cout << "Created " << bricks_.size() << " bricks" << std::endl;
}

void OpenGL::Game::Game::resetGame() {
  gameState_.reset();

  // Reset ball
  if (ball_) {
    ball_->resetToStart();
  }

  // Reset all bricks
  for (auto& brick : bricks_) {
    brick->destroy();  // Clear existing
  }
  bricks_.clear();
  createBrickLayout();  // Recreate

  // Update game state
  gameState_.setRemainingBricks(bricks_.size());
  gameState_.setState(Managers::GameState::PLAYING);

  std::cout << "Game reset!" << std::endl;
}

bool OpenGL::Game::Game::allBricksDestroyed() const {
  for (const auto& brick : bricks_) {
    if (!brick->isDestroyed()) {
      return false;
    }
  }
  return !bricks_.empty();  // Only true if we had bricks and all are destroyed
}