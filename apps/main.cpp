#include <iostream>

#include <game.h>
#include <objects/board_3d.h>
#include <objects/ball.h>  // ADD: Include ball header

int main() {
  std::cout << "Creating Game..." << std::endl;
  auto game = std::make_unique<OpenGL::Game::Game>(800, 800, "3D Game", "shaders/basic.vert",
                                                   "shaders/basic.frag");

  std::cout << "Creating Ball..." << std::endl;
  auto ball = std::make_unique<OpenGL::Game::Objects::Ball>(
      glm::vec3(0.0f, 0.0f, 0.0f),  // Start position (center)
      0.05f                         // Ball radius
  );

  std::cout << "Creating Board..." << std::endl;
  auto board = std::make_unique<OpenGL::Game::Objects::Board3D>();

  std::cout << "Adding Ball to Game..." << std::endl;
  game->setBall(std::move(ball));  // CHANGE: Use setBall() instead of addShape()

  std::cout << "Adding Board to Game..." << std::endl;
  game->setBoard(std::move(board));  // CHANGE: Use setBoard() instead of addShape()

  std::cout << "Starting game..." << std::endl;
  game->run();

  return 0;
}
