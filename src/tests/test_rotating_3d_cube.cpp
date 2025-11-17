#include "test_rotating_3d_cube.hpp"

#include <print>
#include <filesystem>

#include <imgui.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

using namespace festGL;

static constexpr float g_cubeVertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, .0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, .0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, .0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 1.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f
};

TestRotating3DCube::TestRotating3DCube(const std::string &name)
    : Test(name), m_VAO(0), m_VBO(0)
{
    std::filesystem::path vsPath("./../../shaders/3d_perspective.glsl.vert");
    std::filesystem::path fsPath("./../../shaders/3d_perspective.glsl.frag");

    m_shader = IShader::createUnique(vsPath, fsPath);

    glEnable(GL_DEPTH_TEST);

    glCreateVertexArrays(1, &m_VAO);

    glCreateBuffers(1, &m_VBO);
    glNamedBufferStorage(m_VBO, sizeof(g_cubeVertices), g_cubeVertices, GL_DYNAMIC_STORAGE_BIT);
    
    glVertexArrayVertexBuffer(m_VAO, 0, m_VBO, 0, 7 * sizeof(float));

    glEnableVertexArrayAttrib(m_VAO, 0);
    glEnableVertexArrayAttrib(m_VAO, 1);

    glVertexArrayAttribFormat(m_VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(m_VAO, 1, 4, GL_FLOAT, GL_FALSE, 12);

    glVertexArrayAttribBinding(m_VAO, 0, 0);
    glVertexArrayAttribBinding(m_VAO, 1, 0);

    glBindVertexArray(0);
}

TestRotating3DCube::~TestRotating3DCube()
{
    glBindVertexArray(0);

    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
}

void TestRotating3DCube::onUpdate()
{
}

void TestRotating3DCube::onRender()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    m_shader->apply();
    glBindVertexArray(m_VAO);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 0.5f, 0.5f));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

    GLFWwindow* window = glfwGetCurrentContext();
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);

    m_shader->setData<glm::mat4>("uModel", model);
    m_shader->setData<glm::mat4>("uView", view);
    m_shader->setData<glm::mat4>("uProjection", projection);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    // glUseProgram(0);
    glBindTextureUnit(0, 0);
    glBindVertexArray(0);
}

void TestRotating3DCube::onImGuiRender()
{
}