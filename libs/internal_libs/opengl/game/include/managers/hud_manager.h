#ifndef OPENGL_GAME_MANAGERS_HUD_MANAGER_H
#define OPENGL_GAME_MANAGERS_HUD_MANAGER_H

#include "game_state_manager.h"        // Same level (managers/)
#include <string>

namespace OpenGL::Game::Managers {

class HUDManager {
public:
    HUDManager(GameStateManager& gameState);
    ~HUDManager() = default;

    void init();
    void update(float deltaTime);
    void render();
    
    // Display methods
    void showGameStart();
    void showGameOver();
    void showWin();
    void showPaused();
    
    // Settings
    void setUpdateInterval(float interval) { updateInterval_ = interval; }

private:
    void renderGameStatus();
    void renderControls();
    void clearLine();
    void printBorder();
    
    GameStateManager& gameState_;
    
    float updateTimer_;
    float updateInterval_; // How often to update display (seconds)
    bool initialized_;
    
    // Cache previous values to avoid spam
    int lastScore_;
    int lastLives_;
    GameState lastState_;
};

} // namespace OpenGL::Game::Managers

#endif // OPENGL_GAME_MANAGERS_HUD_MANAGER_H