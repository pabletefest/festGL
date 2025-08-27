#include <print>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

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
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
#endif
    const char* glsl_version = "#version 130";
    ImGui_ImplOpenGL3_Init(glsl_version);

 bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

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

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      
      /* Swap front and back buffers */
      glfwSwapBuffers(window);
      
      /* Poll for and process events */
      glfwPollEvents();
  }

  glDeleteProgram(pipelineProgramID);

  glfwTerminate();
  return 0;
}