#ifndef OPENGL_GAME_GAME_H
#define OPENGL_GAME_GAME_H

#include <window.h>
#include <shaders.h>
#include <camera.h>
#include <shape.h>
#include <texture_atlas.h>
#include "objects/ball.h"
#include "objects/brick.h"
#include "objects/board_3d.h"
#include "managers/game_state_manager.h"  // Include managers
#include "managers/collision_manager.h"   // Include managers
#include "managers/hud_manager.h"         // Include managers

#include <memory>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace OpenGL::Game {

class Game : public OpenGL::Core::WindowApp {
 public:
  // Game-tuning constants. Centralized so `createBrickLayout()` and the
  // constructor don't hide the game's defaults behind magic numbers.
  static constexpr int DEFAULT_BRICK_ROWS = 4;
  static constexpr int DEFAULT_BRICK_COLS = 8;
  static constexpr float DEFAULT_BRICK_SPACING = 0.5f;
  static constexpr float DEFAULT_CAMERA_DISTANCE = 5.0f;

  // Path to the texture-atlas JSON config. Loaded eagerly in the ctor so
  // main() can construct shapes against the same atlas before run().
  static constexpr const char* ATLAS_CONFIG_PATH = "textures/atlas.json";

  Game(int width, int height, const char* title, const char* vertexShader,
       const char* fragmentShader);
  ~Game();

  // The texture atlas owned by this Game. Callers (e.g. main()) use this
  // reference to construct textured shapes.
  OpenGL::Core::TextureAtlas& atlas() { return *atlas_; }

  // Object management
  void addShape(std::unique_ptr<OpenGL::Geometry::Shape> shape);
  void addBrick(std::unique_ptr<OpenGL::Game::Objects::Brick> brick);
  void setBall(std::unique_ptr<OpenGL::Game::Objects::Ball> ball);
  void setBoard(std::unique_ptr<OpenGL::Game::Objects::Board3D> board);

  // Core game loop
  void init() override;
  void update(float deltaTime) override;
  void render(float deltaTime) override;
  void processInput() override;

  // Window-event handler (called from glfwSetFramebufferSizeCallback).
  void handleResize(int width, int height);

 private:
  // Loaded eagerly in the ctor (no GL context required — only JSON parsing).
  std::unique_ptr<OpenGL::Core::TextureAtlas> atlas_;

  // Rendering
  OpenGL::Core::Shader shader_;
  OpenGL::Graphics::OrbitCamera camera_;
  std::vector<std::unique_ptr<OpenGL::Geometry::Shape>> shapes_;

  // Game objects
  std::unique_ptr<Objects::Ball> ball_;
  std::unique_ptr<Objects::Board3D> board_;
  std::vector<std::unique_ptr<Objects::Brick>> bricks_;

  // Game systems (using the Managers namespace)
  Managers::GameStateManager gameState_;
  Managers::CollisionManager collisionManager_;
  Managers::HUDManager hud_;

  // Private methods
  void setupUniforms();
  void createBrickLayout();
  void resetGame();
  bool allBricksDestroyed() const;
  void updateGameLogic(float deltaTime);
  void renderGameObjects(float deltaTime);
  void processGameInput();  // Handle game-specific input
};

}  // namespace OpenGL::Game
#endif  // OPENGL_GAME_GAME_H