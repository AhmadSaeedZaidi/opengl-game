#include "game.h"
#include "log.h"
#include <memory>
#include <vector>
#include <iostream>

namespace {

// GLFW framebuffer-size callback. Forwards to the Game instance via the
// window's user pointer so the protected `width`/`height` stay in sync with
// the actual GL viewport.
void framebuffer_size_callback(GLFWwindow* glfw_window, int w, int h) {
  auto* game = static_cast<OpenGL::Game::Game*>(glfwGetWindowUserPointer(glfw_window));
  if (game != nullptr) {
    game->handleResize(w, h);
  }
  glViewport(0, 0, w, h);
}

}  // namespace

OpenGL::Game::Game::Game(int width, int height, const char* title, const char* vertexShader,
                         const char* fragmentShader)
    : OpenGL::Core::WindowApp(width, height, title),
      atlas_(std::make_unique<OpenGL::Core::TextureAtlas>(ATLAS_CONFIG_PATH)),
      shader_(vertexShader, fragmentShader),
      camera_(DEFAULT_CAMERA_DISTANCE),  // Start camera 5 units away
      gameState_(),
      collisionManager_(gameState_),
      hud_(gameState_) {}

OpenGL::Game::Game::~Game() = default;

void OpenGL::Game::Game::setBall(std::unique_ptr<Objects::Ball> ball) { ball_ = std::move(ball); }

void OpenGL::Game::Game::setBoard(std::unique_ptr<Objects::Board3D> board) {
  board_ = std::move(board);
}

void OpenGL::Game::Game::init() {
  OpenGL::Core::WindowApp::init();

  // Register a resize callback so the viewport and aspect ratio stay correct
  // when the user drags the window edges.
  glfwSetWindowUserPointer(window, this);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // Enable depth testing for 3D
  glEnable(GL_DEPTH_TEST);

  // Initialize shader
  shader_.use();

  // Configure shader texture samplers and flags
  shader_.setInt("sidesTexture", 0);
  shader_.setInt("capsTexture", 1);
  shader_.setBool("hasSidesTexture", true);
  shader_.setBool("hasCapsTexture", true);

  // Background plane. Created after the GL context exists so Plane::init() can
  // upload the texture. The plane is square and the texture UVs are cropped to
  // the center 9/16 of the 16:9 source image so the player sees a centered
  // square view that fills the square window. If the region is missing or the
  // file is unreadable the plane still draws (as an untextured quad) — graceful
  // degradation.
  backgroundPlane_ = std::make_unique<OpenGL::Geometry::Plane>(
      glm::vec3(0.0f, 0.0f, BACKGROUND_Z), BACKGROUND_SIZE, BACKGROUND_SIZE, *atlas_,
      BACKGROUND_REGION, BACKGROUND_UV_MIN_U, BACKGROUND_UV_MAX_U, BACKGROUND_UV_MIN_V,
      BACKGROUND_UV_MAX_V);
  backgroundPlane_->init();

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
    // (this handles the kill plane / life loss, the brick bounces,
    // and the paddle bounce — single source of truth.)
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
  // Background first. With depth testing on, the play area will paint over
  // it where their Z ranges overlap; drawing it first also avoids wasting
  // fragment work on areas the play area will overwrite later.
  if (backgroundPlane_) {
    backgroundPlane_->draw(shader_.ID, deltaTime, window);
  }

  // Render game objects
  if (ball_) ball_->draw(shader_.ID, deltaTime, window);
  if (board_) board_->draw(shader_.ID, deltaTime, window);
  for (auto& brick : bricks_) {
    if (!brick->isDestroyed()) {
      brick->draw(shader_.ID, deltaTime, window);
    }
  }
}

void OpenGL::Game::Game::processInput() {
  // Base class escape key handling
  OpenGL::Core::WindowApp::processInput();
}

void OpenGL::Game::Game::handleResize(int w, int h) {
  width = w;
  height = h;
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

  // Pause/unpause with P key (PLAYING <-> PAUSED), or restart from a terminal
  // state (GAME_OVER / WON). Space already restarts from any state, so this
  // gives a second consistent way to keep playing.
  if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
    if (!pPressed) {
      const auto state = gameState_.getState();
      if (state == Managers::GameState::PLAYING) {
        gameState_.setState(Managers::GameState::PAUSED);
#if OPENGL_VERBOSE_LOG
        std::cout << "Game PAUSED" << std::endl;
#endif
      } else if (state == Managers::GameState::PAUSED) {
        gameState_.setState(Managers::GameState::PLAYING);
#if OPENGL_VERBOSE_LOG
        std::cout << "Game RESUMED" << std::endl;
#endif
      } else if (state == Managers::GameState::GAME_OVER ||
                 state == Managers::GameState::WON) {
        resetGame();
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
  const int rows = DEFAULT_BRICK_ROWS;
  const int cols = DEFAULT_BRICK_COLS;
  const float brickSpacing = DEFAULT_BRICK_SPACING;
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

      auto brick = std::make_unique<Objects::Brick>(position, *atlas_, "brick_sides", "brick_caps",
                                                     color);
      brick->init();
      bricks_.emplace_back(std::move(brick));
    }
  }

#if OPENGL_VERBOSE_LOG
  std::cout << "Created " << bricks_.size() << " bricks" << std::endl;
#endif
}

void OpenGL::Game::Game::resetGame() {
  gameState_.reset();

  // Reset ball
  if (ball_) {
    ball_->resetToStart();
  }

  // Reset paddle speed boost (paddleHits_ counter and currentSpeed_).
  if (board_) {
    board_->resetSpeed();
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

#if OPENGL_VERBOSE_LOG
  std::cout << "Game reset!" << std::endl;
#endif
}

bool OpenGL::Game::Game::allBricksDestroyed() const {
  for (const auto& brick : bricks_) {
    if (!brick->isDestroyed()) {
      return false;
    }
  }
  return !bricks_.empty();  // Only true if we had bricks and all are destroyed
}