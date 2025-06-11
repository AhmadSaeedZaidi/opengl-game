#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shaders.h>
#include <iostream>

unsigned int VAO;            // global variable to hold the Vertex Array Object ID

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void clearScreen() {
  // clear the color buffer
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

void swapBuffers(GLFWwindow* window) {
  glfwSwapBuffers(window);
  glfwPollEvents();
}

void rendershape(float* vertices, unsigned int* indices, int sizeofvertices, int sizeofindices) {
  unsigned int VBO, EBO;       // Vertex Buffer Object and Element Buffer Object
  glGenBuffers(1, &VBO);       // assigned an id to the buffer object
  glGenBuffers(1, &EBO);       // assigned an id to the element buffer object
  glGenVertexArrays(1, &VAO);  // assigned an id to the vertex array object

  glBindVertexArray(VAO);              // bind the vertex array object
  glBindBuffer(GL_ARRAY_BUFFER, VBO);  // bind the buffer object to the GL_ARRAY_BUFFER target
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);  // bind the element buffer object

  // copy the vertex data to the buffer objects
  glBufferData(GL_ARRAY_BUFFER, sizeofvertices, vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeofindices, indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
}

void rectangle() {
  float vertices[] = {
      0.5f,  0.5f,  0.0f,  // top right
      0.5f,  -0.5f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f,  // bottom left
      -0.5f, 0.5f,  0.0f   // top left
  };
  unsigned int indices[] = {
      // note that we start from 0!
      0, 1, 3,  // first triangle
      1, 2, 3   // second triangle
  };
  rendershape(vertices, indices, sizeof(vertices), sizeof(indices));
}
int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  glViewport(0, 0, 800, 600);
  //  new shader class:
  Shader* myshaders;
  myshaders = new Shader("shaders/basic.vert", "shaders/basic.frag");
  rectangle();  // call the function to create the triangle
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // clear the color buffer
    clearScreen();

    // draw the triangle
    myshaders->use();        // use the shader program we created earlier
    glBindVertexArray(VAO);  // bind the vertex array object to use its state
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  // draw the triangle

    // swap the buffers and poll for events
    swapBuffers(window);
  }

  glfwTerminate();
  return 0;
}
