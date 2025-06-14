#ifndef GAME
#define GAME

#include "window.h"
#include "shaders.h"
#include "camera.h"
#include "shape.h"
#include <memory>
#include <vector>

class Game : public WindowApp {
 public:
  Game(int width, int height, const char* title, const char* vertexShader,
       const char* fragmentShader);
  ~Game();

  void addShape(std::unique_ptr<Shape> shape);

  void init() override;
  void update(float deltaTime) override;
  void render(float deltaTime) override;
  void processInput() override;

 private:
  Shader shader_;
  OrbitCamera camera_;
  std::vector<std::unique_ptr<Shape>> shapes_;

  void setupUniforms();
};

#endif  // GAME_H