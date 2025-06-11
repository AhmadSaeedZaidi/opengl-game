#include <gtest/gtest.h>
#include "shaders.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>

// Set up an OpenGL context for testing
class ShaderTest : public ::testing::Test {
 protected:
  GLFWwindow* window;

  void SetUp() override {
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);  // Hidden window

    // Create window
    window = glfwCreateWindow(800, 600, "Shader Test", NULL, NULL);
    ASSERT_TRUE(window != nullptr) << "Failed to create GLFW window";
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    ASSERT_TRUE(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) << "Failed to initialize GLAD";
  }

  void TearDown() override {
    glfwDestroyWindow(window);
    glfwTerminate();
  }

  // Helper to create temporary shader files
  void createTempShaderFile(const std::string& path, const std::string& content) {
    std::ofstream file(path);
    file << content;
    file.close();
  }
};

// Test shader constructor with valid files
TEST_F(ShaderTest, ConstructorWithValidFiles) {
  // Create valid shader files
  const char* vertPath = "temp_valid_vert.glsl";
  const char* fragPath = "temp_valid_frag.glsl";

  createTempShaderFile(vertPath,
                       "#version 330 core\n"
                       "layout (location = 0) in vec3 aPos;\n"
                       "void main() {\n"
                       "    gl_Position = vec4(aPos, 1.0);\n"
                       "}");

  createTempShaderFile(fragPath,
                       "#version 330 core\n"
                       "out vec4 FragColor;\n"
                       "void main() {\n"
                       "    FragColor = vec4(1.0, 0.5, 0.2, 1.0);\n"
                       "}");

  // This should not throw
  EXPECT_NO_THROW({
    Shader shader(vertPath, fragPath);
    EXPECT_NE(shader.ID, 0) << "Shader program ID should not be 0";
  });

  // Clean up
  std::remove(vertPath);
  std::remove(fragPath);
}

// Test shader constructor with invalid syntax
TEST_F(ShaderTest, ConstructorWithInvalidSyntax) {
  const char* vertPath = "temp_invalid_vert.glsl";
  const char* fragPath = "temp_invalid_frag.glsl";

  createTempShaderFile(vertPath,
                       "#version 330 core\n"
                       "layout (location = 0) in vec3 aPos;\n"
                       "void main() {\n"
                       "    gl_Position = vec4(aPos, 1.0)\n"  // Missing semicolon
                       "}");

  createTempShaderFile(fragPath,
                       "#version 330 core\n"
                       "out vec4 FragColor;\n"
                       "void main() {\n"
                       "    FragColor = vec4(1.0, 0.5, 0.2, 1.0);\n"
                       "}");

  // We expect shader compilation to fail but not crash
  testing::internal::CaptureStdout();
  Shader shader(vertPath, fragPath);
  std::string output = testing::internal::GetCapturedStdout();

  EXPECT_TRUE(output.find("ERROR::SHADER::VERTEX::COMPILATION_FAILED") != std::string::npos)
      << "Expected compilation error message";

  // Clean up
  std::remove(vertPath);
  std::remove(fragPath);
}

// Test shader constructor with missing files
TEST_F(ShaderTest, ConstructorWithMissingFiles) {
  // Non-existent files
  const char* vertPath = "non_existent_vert.glsl";
  const char* fragPath = "non_existent_frag.glsl";

  testing::internal::CaptureStdout();
  EXPECT_ANY_THROW({ Shader shader(vertPath, fragPath); });
  std::string output = testing::internal::GetCapturedStdout();

  EXPECT_TRUE(output.find("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ") != std::string::npos)
      << "Expected file read error message";
}

// Test uniform methods
TEST_F(ShaderTest, UniformMethods) {
  // Create valid shader files with a uniform
  const char* vertPath = "temp_uniform_vert.glsl";
  const char* fragPath = "temp_uniform_frag.glsl";

  createTempShaderFile(vertPath,
                       "#version 330 core\n"
                       "layout (location = 0) in vec3 aPos;\n"
                       "uniform bool testBool;\n"
                       "uniform int testInt;\n"
                       "uniform float testFloat;\n"
                       "void main() {\n"
                       "    gl_Position = vec4(aPos, 1.0);\n"
                       "}");

  createTempShaderFile(fragPath,
                       "#version 330 core\n"
                       "out vec4 FragColor;\n"
                       "void main() {\n"
                       "    FragColor = vec4(1.0, 0.5, 0.2, 1.0);\n"
                       "}");

  Shader shader(vertPath, fragPath);

  // Testing uniform setters - these should not crash
  EXPECT_NO_THROW({
    shader.use();
    shader.setBool("testBool", true);
    shader.setInt("testInt", 42);
    shader.setFloat("testFloat", 3.14f);
  });

  // Clean up
  std::remove(vertPath);
  std::remove(fragPath);
}

// Make sure uniforms with invalid names don't crash
TEST_F(ShaderTest, InvalidUniformNames) {
  // Create valid shader files
  const char* vertPath = "temp_basic_vert.glsl";
  const char* fragPath = "temp_basic_frag.glsl";

  createTempShaderFile(vertPath,
                       "#version 330 core\n"
                       "layout (location = 0) in vec3 aPos;\n"
                       "void main() {\n"
                       "    gl_Position = vec4(aPos, 1.0);\n"
                       "}");

  createTempShaderFile(fragPath,
                       "#version 330 core\n"
                       "out vec4 FragColor;\n"
                       "void main() {\n"
                       "    FragColor = vec4(1.0, 0.5, 0.2, 1.0);\n"
                       "}");

  Shader shader(vertPath, fragPath);

  // These should not crash even though "nonExistentUniform" doesn't exist
  EXPECT_NO_THROW({
    shader.use();
    shader.setBool("nonExistentUniform", true);
    shader.setInt("nonExistentUniform", 42);
    shader.setFloat("nonExistentUniform", 3.14f);
  });

  // Clean up
  std::remove(vertPath);
  std::remove(fragPath);
}

// Run the tests
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}