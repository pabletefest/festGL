#include <print>

#include <Glad/glad.h>
#include <GLFW/glfw3.h>

#include <filesystem>
#include <fstream>
#include <iterator>

static std::string readShaderFile(std::filesystem::path filepath) {
  std::ifstream fileStream{ filepath, std::ios::binary };
  return { std::istreambuf_iterator{fileStream}, {} };
}

int main() {
  std::println("Hello triangle!");

  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
      return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello Triangle!", NULL, NULL);
  if (!window)
  {
      glfwTerminate();
      return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::println("Failed to initialize GLAD");
    return -1;
  }

  std::println("Vendor: {}", (const char *)glGetString(GL_VENDOR));
  std::println("Renderer: {}", (const char *)glGetString(GL_RENDERER));
  std::println("Version: {}", (const char *)glGetString(GL_VERSION));

  GLuint pipelineProgramID = glCreateProgram();

  std::string vs = readShaderFile("../../../src/shaders/shader.vert");
  std::string fs = readShaderFile("../../../src/shaders/shader.frag");

  std::println("\nVERTEX SHADER: \n{}", vs);
  std::println("\nFRAGMENT SHADER: \n{}", fs);

  GLuint vsShaderID = glCreateShader(GL_VERTEX_SHADER);
  const char* vsShaderSource = vs.c_str();
  glShaderSource(vsShaderID, 1, &vsShaderSource, nullptr);
  glCompileShader(vsShaderID);

  GLint result;
  glGetShaderiv(vsShaderID, GL_COMPILE_STATUS, &result);

  if (!result) {
    GLsizei length;
    char message[1024];
    glGetShaderInfoLog(vsShaderID, sizeof(message), &length, message);
    std::println("Error compiling shader: {}!", message);
    glDeleteShader(vsShaderID);
    exit(1);
  }

  GLuint fsShaderID = glCreateShader(GL_FRAGMENT_SHADER);
  const char* fsShaderSource = fs.c_str();
  glShaderSource(fsShaderID, 1, &fsShaderSource, nullptr);
  glCompileShader(fsShaderID);

  glGetShaderiv(fsShaderID, GL_COMPILE_STATUS, &result);

  if (!result) {
    GLsizei length;
    char message[1024];
    glGetShaderInfoLog(fsShaderID, sizeof(message), &length, message);
    std::println("Error compiling shader: {}!", message);
    glDeleteShader(fsShaderID);
    exit(1);
  }

  glAttachShader(pipelineProgramID, vsShaderID);
  glAttachShader(pipelineProgramID, fsShaderID);

  glLinkProgram(pipelineProgramID);

  glValidateProgram(pipelineProgramID);

  glGetProgramiv(pipelineProgramID, GL_VALIDATE_STATUS, &result);

  if (!result) {
    GLsizei length;
    char message[1024];
    glGetProgramInfoLog(pipelineProgramID, sizeof(message), &length, message);
    std::println("Error validating shader program: {}!", message);
    glDeleteProgram(pipelineProgramID);
    exit(1);
  }

  glUseProgram(pipelineProgramID);

  glDetachShader(pipelineProgramID, vsShaderID);
  glDetachShader(pipelineProgramID, fsShaderID);
  glDeleteShader(vsShaderID);
  glDeleteShader(fsShaderID);

  float trianglePositions[] = {
    -1.0f, -1.0f,
     0.0f,  1.0f,
     1.0f, -1.0f
  };

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(trianglePositions), trianglePositions, GL_STATIC_DRAW);
  
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

  glBindVertexArray(0);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
      /* Render here */
      glClear(GL_COLOR_BUFFER_BIT);

      glBindVertexArray(vao);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      
      /* Swap front and back buffers */
      glfwSwapBuffers(window);
      
      /* Poll for and process events */
      glfwPollEvents();
  }

  glDeleteProgram(pipelineProgramID);

  glfwTerminate();
  return 0;
}