#include "managers/hud_manager.h"
#include <iostream>
#include <iomanip>

OpenGL::Game::Managers::HUDManager::HUDManager(GameStateManager& gameState)
    : gameState_(gameState),
      updateTimer_(0.0f),
      updateInterval_(0.5f),  // Update every 0.5 seconds
      initialized_(false),
      lastScore_(-1),
      lastLives_(-1),
      lastState_(GameState::PLAYING)
{
}

void OpenGL::Game::Managers::HUDManager::init() {
    if (initialized_) return;
    
    std::cout << "\n";
    printBorder();
    std::cout << "|                 BREAKOUT 3D                 |\n";
    printBorder();
    std::cout << "| Controls:                                   |\n";
    std::cout << "| - Left/Right Arrow or A/D: Move paddle     |\n";
    std::cout << "| - Left Mouse + Drag: Rotate camera         |\n";
    std::cout << "| - R: Reset camera view                      |\n";
    std::cout << "| - ESC: Quit game                            |\n";
    printBorder();
    std::cout << "\n";
    
    initialized_ = true;
    
    // Force first update
    lastScore_ = -1;
    lastLives_ = -1;
    lastState_ = static_cast<GameState>(-1);
}

void OpenGL::Game::Managers::HUDManager::update(float deltaTime) {
    updateTimer_ += deltaTime;
    
    // Only update display at specified intervals
    if (updateTimer_ >= updateInterval_) {
        updateTimer_ = 0.0f;
        
        // Check if any values changed
        bool needsUpdate = false;
        
        if (gameState_.getScore() != lastScore_ ||
            gameState_.getLives() != lastLives_ ||
            gameState_.getCurrentState() != lastState_) {
            needsUpdate = true;
        }
        
        if (needsUpdate) {
            renderGameStatus();
            
            // Update cached values
            lastScore_ = gameState_.getScore();
            lastLives_ = gameState_.getLives();
            lastState_ = gameState_.getCurrentState();
        }
    }
}

void OpenGL::Game::Managers::HUDManager::render() {
    // Handle state-specific displays
    switch (gameState_.getCurrentState()) {
        case GameState::GAME_OVER:
            showGameOver();
            break;
        case GameState::WON:
            showWin();
            break;
        case GameState::PAUSED:
            showPaused();
            break;
        case GameState::PLAYING:
            // Regular status is handled in update()
            break;
    }
}

void OpenGL::Game::Managers::HUDManager::showGameStart() {
    clearLine();
    std::cout << "\n";
    printBorder();
    std::cout << "|              GAME STARTING!                 |\n";
    std::cout << "|          Destroy all the bricks!            |\n";
    printBorder();
    std::cout << "\n";
}

void OpenGL::Game::Managers::HUDManager::showGameOver() {
    static bool shown = false;
    if (shown) return;
    
    clearLine();
    std::cout << "\n";
    printBorder();
    std::cout << "|                GAME OVER!                   |\n";
    std::cout << "| Final Score: " << std::setw(26) << gameState_.getScore() << " |\n";
    std::cout << "|          Press ESC to quit                  |\n";
    printBorder();
    std::cout << "\n";
    
    shown = true;
}

void OpenGL::Game::Managers::HUDManager::showWin() {
    static bool shown = false;
    if (shown) return;
    
    clearLine();
    std::cout << "\n";
    printBorder();
    std::cout << "|            CONGRATULATIONS!                 |\n";
    std::cout << "|           YOU WIN! ALL BRICKS               |\n";
    std::cout << "|             DESTROYED!                      |\n";
    std::cout << "| Final Score: " << std::setw(26) << gameState_.getScore() << " |\n";
    std::cout << "|          Press ESC to quit                  |\n";
    printBorder();
    std::cout << "\n";
    
    shown = true;
}

void OpenGL::Game::Managers::HUDManager::showPaused() {
    static bool shown = false;
    if (shown) return;
    
    clearLine();
    std::cout << "\n";
    printBorder();
    std::cout << "|               GAME PAUSED                   |\n";
    std::cout << "|        Press P to resume game               |\n";
    printBorder();
    std::cout << "\n";
    
    shown = true;
}

void OpenGL::Game::Managers::HUDManager::renderGameStatus() {
    if (!initialized_) return;
    
    std::cout << "\r"; // Return to beginning of line
    std::cout << "Score: " << std::setw(6) << gameState_.getScore() 
              << " | Lives: " << std::setw(2) << gameState_.getLives()
              << " | Bricks: " << std::setw(3) << gameState_.getRemainingBricks()
              << " | State: ";
    
    switch (gameState_.getCurrentState()) {
        case GameState::PLAYING:
            std::cout << "PLAYING ";
            break;
        case GameState::GAME_OVER:
            std::cout << "GAME OVER";
            break;
        case GameState::WON:
            std::cout << "WON     ";
            break;
        case GameState::PAUSED:
            std::cout << "PAUSED  ";
            break;
    }
    
    std::cout << std::flush; // Force output
}

void OpenGL::Game::Managers::HUDManager::renderControls() {
    std::cout << "\nControls: [A/D] or [←/→] Move Paddle | [Mouse] Camera | [R] Reset View | [ESC] Quit\n";
}

void OpenGL::Game::Managers::HUDManager::clearLine() {
    std::cout << "\r" << std::string(80, ' ') << "\r" << std::flush;
}

void OpenGL::Game::Managers::HUDManager::printBorder() {
    std::cout << "+---------------------------------------------+\n";
}