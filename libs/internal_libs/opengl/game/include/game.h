#ifndef GAME
#define GAME

#include <window.h>
#include <shaders.h>
#include <camera.h>
#include <shape.h>

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

  void addShape(std::unique_ptr<OpenGL::Geometry::Shape> shape);

  void init() override;
  void update(float deltaTime) override;
  void render(float deltaTime) override;
  void processInput() override;

 private:
  OpenGL::Core::Shader shader_;
  OpenGL::Graphics::OrbitCamera camera_;
  std::vector<std::unique_ptr<OpenGL::Geometry::Shape>> shapes_;

  void setupUniforms();
};
}  // namespace OpenGL::Game
#endif  // GAME_H