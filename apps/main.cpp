#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shaders.h>
#include <window.h>
#include <iostream>

class RenderWindow : public WindowApp, public Shader {
  // RenderWindow class inherits from WindowApp and Shader
  // This class will handle the rendering of a rectangle using OpenGL
  // It uses the shader program created in the Shader class
 public:
  RenderWindow(int width, int height, const char* title, const char* vS, const char* fS)
      : WindowApp(width, height, title), Shader(vS, fS) {}
  void update() override {
    use();                   // use the shader program we created earlier
    glBindVertexArray(VAO);  // bind the vertex array object to use its state
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  // draw the rectangle
  }  // override the update method
  void init() override {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // 2) Fill VBO, EBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 3) Setup attributes
    // position → layout(location = 0), first 3 floats
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color → layout(location = 1), next 3 floats
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 4) Unbind VAO to avoid accidental state changes
    glBindVertexArray(0);
  }

 private:
  // make these persist
  unsigned int VAO = 0;
  unsigned int VBO = 0;
  unsigned int EBO = 0;

  // you can keep these here if you like
  float vertices[24] = {
      // positions        // colors
      0.5f,  0.5f,  0.0f, 1, 0, 0,  // top right
      0.5f,  -0.5f, 0.0f, 0, 1, 0,  // bottom right
      -0.5f, -0.5f, 0.0f, 0, 0, 1,  // bottom left
      -0.5f, 0.5f,  0.0f, 1, 1, 0   // top left
  };
  unsigned int indices[6] = {0, 1, 3, 1, 2, 3};
};

int main() {
  RenderWindow* window;  // create a window instance
  window = new RenderWindow(800, 600, "LearnOpenGL", "shaders/basic.vert", "shaders/basic.frag");
  window->run();  // run the window application
  return 0;
}
