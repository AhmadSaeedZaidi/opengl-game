#ifndef OPENGL_GAME_MANAGERS_GAME_STATE_MANAGER_H
#define OPENGL_GAME_MANAGERS_GAME_STATE_MANAGER_H

#include <iostream>

namespace OpenGL::Game::Managers {

enum class GameState { PLAYING, GAME_OVER, WON, PAUSED };

class GameStateManager {
 public:
  GameStateManager();

  // State management
  GameState getState() const { return state_; }
  GameState getCurrentState() const { return state_; } 
  void setState(GameState state);
  bool isPlaying() const { return state_ == GameState::PLAYING; }
  bool isPaused() const { return state_ == GameState::PAUSED; }
  bool isGameOver() const { return state_ == GameState::GAME_OVER; }
  bool hasWon() const { return state_ == GameState::WON; }

  // Lives management
  int getLives() const { return lives_; }
  void setLives(int lives) { lives_ = lives; }
  void loseLife();
  bool hasLives() const { return lives_ > 0; }

  // Score management
  int getScore() const { return score_; }
  void addScore(int points);
  void resetScore() { score_ = 0; }

  // Game statistics
  int getBricksDestroyed() const { return bricksDestroyed_; }
  void incrementBricksDestroyed() { bricksDestroyed_++; }
  void resetBricksDestroyed() { bricksDestroyed_ = 0; }
  void setRemainingBricks(int count) { remainingBricks_ = count; }
  int getRemainingBricks() const { return remainingBricks_; }

  // Reset everything
  void reset();

  // Display current state
  void printStatus() const;

 private:
  GameState state_;
  int lives_;
  int score_;
  int bricksDestroyed_;
  int remainingBricks_ = 0;
};

}  // namespace OpenGL::Game::Managers

#endif  // OPENGL_GAME_MANAGERS_GAME_STATE_MANAGER_H