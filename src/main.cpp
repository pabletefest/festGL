#include <print>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <Glad/glad.h>
#include <GLFW/glfw3.h>

#include <filesystem>
#include <fstream>
#include <iterator>

#include "tests/tests_menu.hpp"
#include "tests/test_clear_color.hpp"

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
  window = glfwCreateWindow(640, 480, "OpenGL Renderer", NULL, NULL);
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

  float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()); // Valid on GLFW 3.3+ only
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsLight();

  // Setup scaling
  ImGuiStyle& style = ImGui::GetStyle();
  style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
  style.FontScaleDpi = main_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);

  const char* glsl_version = "#version 330";
  ImGui_ImplOpenGL3_Init(glsl_version);

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

  TestsMenu testsMenu = TestsMenu("Tests Menu");
  testsMenu.registerTest<TestClearColor>("Clear color");
  std::string panelTitle = testsMenu.getTestName();

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
      /* Render here */
      glClear(GL_COLOR_BUFFER_BIT);

      glBindVertexArray(vao);
      glDrawArrays(GL_TRIANGLES, 0, 3);

      if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
      {
          ImGui_ImplGlfw_Sleep(10);
          continue;
      }

      // Start the Dear ImGui frame
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      Test* currentTest = testsMenu.getCurrentTest();
      
      if (currentTest) {
        currentTest->onUpdate();
        currentTest->onRender();
        
        panelTitle = currentTest->getTestName();

        ImGui::Begin(panelTitle.c_str());

        if (currentTest != &testsMenu && ImGui::Button("<-")) {
          // panelTitle = currentTest->getTestName() + std::format(" (:.2{0}f ms/frame - :{1}f FPS)", 1000.0f / io.Framerate, io.Framerate);
          currentTest = &testsMenu;
          testsMenu.setCurrentTest(currentTest);
        }

        currentTest->onImGuiRender();

        ImGui::End();
      }
      
      // Rendering
      ImGui::Render();
      int display_w, display_h;
      glfwGetFramebufferSize(window, &display_w, &display_h);
      glViewport(0, 0, display_w, display_h);
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      
      /* Swap front and back buffers */
      glfwSwapBuffers(window);
      
      /* Poll for and process events */
      glfwPollEvents();
  }

  glDeleteProgram(pipelineProgramID);

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwTerminate();

  return 0;
}