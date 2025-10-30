#include "test_hello_triangle.hpp"

#include <print>
#include <filesystem>
#include <fstream>
#include <iterator>

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

static std::string readShaderFile(std::filesystem::path filepath) {
  std::ifstream fileStream{ filepath, std::ios::binary };
  return { std::istreambuf_iterator{fileStream}, {} };
}

static const GLfloat g_trianglePositions[] = {
   -1.0f, -1.0f,
    0.0f,  1.0f,
    1.0f, -1.0f
};

static const GLuint g_triangleIndexes[] = {
    0, 2, 1
};

TestHelloTriangle::TestHelloTriangle(const std::string &name)
    : Test(name), m_VAO(0), m_VBO(0), m_IBO(0), m_pipelineProgramID(0), m_triangleColor({ 1.0f, 0.0f, 0.0f, 1.0f})
{
    m_pipelineProgramID= glCreateProgram();

    std::string vs = readShaderFile("./shaders/color.glsl.vert");
    std::string fs = readShaderFile("./shaders/color.glsl.frag");

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

    glAttachShader(m_pipelineProgramID, vsShaderID);
    glAttachShader(m_pipelineProgramID, fsShaderID);

    glLinkProgram(m_pipelineProgramID);

    glValidateProgram(m_pipelineProgramID);

    glGetProgramiv(m_pipelineProgramID, GL_VALIDATE_STATUS, &result);

    if (!result) {
        GLsizei length;
        char message[1024];
        glGetProgramInfoLog(m_pipelineProgramID, sizeof(message), &length, message);
        std::println("Error validating shader program: {}!", message);
        glDeleteProgram(m_pipelineProgramID);
        exit(1);
    }

    glUseProgram(m_pipelineProgramID);

    glDetachShader(m_pipelineProgramID, vsShaderID);
    glDetachShader(m_pipelineProgramID, fsShaderID);
    glDeleteShader(vsShaderID);
    glDeleteShader(fsShaderID);

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_trianglePositions), g_trianglePositions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    glGenBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_triangleIndexes), g_triangleIndexes, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

TestHelloTriangle::~TestHelloTriangle()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteProgram(m_pipelineProgramID);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
}

void TestHelloTriangle::onUpdate()
{
    GLint location = glGetUniformLocation(m_pipelineProgramID, "uColor");
    glUniform4fv(location, 1, glm::value_ptr(m_triangleColor));
}

void TestHelloTriangle::onRender()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(m_VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, sizeof(g_trianglePositions)/sizeof(g_triangleIndexes[0]), GL_UNSIGNED_INT, nullptr);
}

void TestHelloTriangle::onImGuiRender()
{
    ImGui::ColorEdit4("Triangle color", glm::value_ptr(m_triangleColor));
}
