#ifndef GAME
#define GAME

#include <window.h>
#include <shaders.h>
#include <camera.h>
#include <shape.h>
#include "objects/ball.h"
#include "objects/brick.h"
#include "objects/board_3d.h"
#include "managers/game_state_manager.h"  // Include managers
#include "managers/collision_manager.h"   // Include managers
#include "managers/hud_manager.h"         // Include managers

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace OpenGL::Game {

class Game : public OpenGL::Core::WindowApp {
 public:
  Game(int width, int height, const char* title, const char* vertexShader,
       const char* fragmentShader);
  ~Game();

  // Object management
  void addShape(std::unique_ptr<OpenGL::Geometry::Shape> shape);
  void addBrick(std::unique_ptr<Objects::Brick> brick);
  void setBall(std::unique_ptr<Objects::Ball> ball);
  void setBoard(std::unique_ptr<Objects::Board3D> board);

  // Core game loop
  void init() override;
  void update(float deltaTime) override;
  void render(float deltaTime) override;
  void processInput() override;

 private:
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
#endif  // GAME