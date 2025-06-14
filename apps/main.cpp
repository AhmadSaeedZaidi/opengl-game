#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <game.h>
#include <board_3d.h>

int main() {
  std::cout << "Creating Game..." << std::endl;
  auto game =
      std::make_unique<Game>(800, 600, "3D Game", "shaders/basic.vert", "shaders/basic.frag");

  std::cout << "Creating Board..." << std::endl;
  auto board = std::make_unique<Board3D>();

  // Add this debug
  std::cout << "Board created, type info: " << typeid(*board).name() << std::endl;

  std::cout << "Adding Board to Game..." << std::endl;
  game->addShape(std::move(board));

  std::cout << "Starting game..." << std::endl;
  game->run();

  return 0;
}
