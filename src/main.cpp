#include <print>

#include <imgui.h>
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "tests/tests_menu.hpp"
#include "tests/test_clear_color.hpp"
#include "tests/test_hello_triangle.hpp"
#include "tests/test_texture2D_square.hpp"
#include "tests/test_rotating_3d_cube.hpp"

#include "utils/image_loader.hpp"

static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

int main() {
  std::println("Hello OpenGL renderer!");

  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
      return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "festGL renderer", NULL, NULL);
  
  if (!window)
  {
      glfwTerminate();
      return -1;
  }

  GLFWimage icons[1];
  int channels;
  std::filesystem::path iconPath = "./../../resources/festGL_transparent_logo.png";

  icons[0].pixels = festGL::ImageLoader::load<festGL::ImageLoader::FlipType::NoFlip>(iconPath, icons[0].width, icons[0].height, channels);
  glfwSetWindowIcon(window, 1, icons);
  stbi_image_free(icons[0].pixels);

  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  if (!gladLoadGL(glfwGetProcAddress))
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

  const char* glsl_version = "#version 450";
  ImGui_ImplOpenGL3_Init(glsl_version);

  std::println("Vendor: {}", (const char *)glGetString(GL_VENDOR));
  std::println("Renderer: {}", (const char *)glGetString(GL_RENDERER));
  std::println("Version: {}", (const char *)glGetString(GL_VERSION));

  TestsMenu testsMenu = TestsMenu("Tests Menu");
  testsMenu.registerTest<TestClearColor>("Clear color");
  testsMenu.registerTest<TestHelloTriangle>("Hello triangle");
  testsMenu.registerTest<TestText2DSquare>("Texture2D Quad");
  testsMenu.registerTest<TestRotating3DCube>("3D Cube rotating");

  std::string panelTitle = testsMenu.getTestName();

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
      /* Render here */
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

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwTerminate();

  return 0;
}