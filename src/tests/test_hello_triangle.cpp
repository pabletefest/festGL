#include "test_hello_triangle.hpp"
#include "file_reader.hpp"

#include <print>
#include <filesystem>
#include <fstream>
#include <iterator>

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

using namespace festGL;

static const GLfloat g_trianglePositions[] = {
   -1.0f, -1.0f,
    0.0f,  1.0f,
    1.0f, -1.0f
};

static const GLuint g_triangleIndexes[] = {
    0, 2, 1
};

TestHelloTriangle::TestHelloTriangle(const std::string &name)
    : Test(name), m_VAO(0), m_VBO(0), m_IBO(0), m_triangleColor({ 1.0f, 0.0f, 0.0f, 1.0f})
{
    std::filesystem::path vsPath = "./../../shaders/color.glsl.vert";
    std::filesystem::path fsPath = "./../../shaders/color.glsl.frag";

    m_shader = IShader::createUnique(vsPath, fsPath);

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

    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
}

void TestHelloTriangle::onUpdate()
{
}

void TestHelloTriangle::onRender()
{
    glClear(GL_COLOR_BUFFER_BIT);

    m_shader->apply();
    m_shader->setDataBuffer<glm::vec4>("uColor", { m_triangleColor } );

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, sizeof(g_triangleIndexes)/sizeof(g_triangleIndexes[0]), GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
}

void TestHelloTriangle::onImGuiRender()
{
    ImGui::ColorEdit4("Triangle color", glm::value_ptr(m_triangleColor));
}
