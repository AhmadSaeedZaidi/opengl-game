#include "managers/game_state_manager.h"

OpenGL::Game::Managers::GameStateManager::GameStateManager()
    : state_(GameState::PLAYING),
      lives_(3),
      score_(0),
      bricksDestroyed_(0),
      remainingBricks_(0) {  // ADD: remainingBricks_(0)
  std::cout << "Game State Manager initialized" << std::endl;
}

void OpenGL::Game::Managers::GameStateManager::setState(GameState state) {
  GameState oldState = state_;
  state_ = state;

  if (oldState != state) {
    switch (state) {
      case GameState::PLAYING:
        std::cout << "Game State: PLAYING" << std::endl;
        break;
      case GameState::PAUSED:
        std::cout << "Game State: PAUSED" << std::endl;
        break;
      case GameState::GAME_OVER:
        std::cout << "Game State: GAME OVER" << std::endl;
        break;
      case GameState::WON:
        std::cout << "Game State: YOU WIN!" << std::endl;
        break;
    }
  }
}

void OpenGL::Game::Managers::GameStateManager::loseLife() {
  lives_--;
  std::cout << "Life lost! Lives remaining: " << lives_ << std::endl;

  if (lives_ <= 0) {
    setState(GameState::GAME_OVER);
  }
}

void OpenGL::Game::Managers::GameStateManager::addScore(int points) {
  score_ += points;
  std::cout << "Score: " << score_ << " (+" << points << ")" << std::endl;
}

void OpenGL::Game::Managers::GameStateManager::reset() {
  state_ = GameState::PLAYING;
  lives_ = 3;
  score_ = 0;
  bricksDestroyed_ = 0;
  remainingBricks_ = 0;  // ADD: Reset remaining bricks
  std::cout << "Game state reset! Lives: " << lives_ << " | Score: " << score_ << std::endl;
}

void OpenGL::Game::Managers::GameStateManager::printStatus() const {
  std::cout << "Status: Lives=" << lives_ << " Score=" << score_ << " Bricks=" << bricksDestroyed_
            << " Remaining=" << remainingBricks_ << " State=";
  switch (state_) {
    case GameState::PLAYING:
      std::cout << "PLAYING";
      break;
    case GameState::PAUSED:
      std::cout << "PAUSED";
      break;
    case GameState::GAME_OVER:
      std::cout << "GAME_OVER";
      break;
    case GameState::WON:
      std::cout << "WON";
      break;
  }
  std::cout << std::endl;
}