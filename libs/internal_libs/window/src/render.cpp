#include <glad/glad.h>
#include <render.h>

RenderWindow::RenderWindow(int width, int height, const char* title, const char* vS,
                           const char* fS)
    : WindowApp(width, height, title), Shader(vS, fS) {}
void RenderWindow::update() {
  use();                   // use the shader program we created earlier
  glBindVertexArray(VAO);  // bind the vertex array object to use its state
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  // draw the rectangle
}
void RenderWindow::init() {
  WindowApp::init();  // call the base class init method
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

RenderWindow::~RenderWindow() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}