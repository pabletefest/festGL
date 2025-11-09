#pragma once

#include "test.hpp"
#include "renderer/shader.hpp"

#include <glad/gl.h>
#include <glm/vec4.hpp>

#include <memory>

using namespace festGL;

class TestHelloTriangle : public Test {
public:
    TestHelloTriangle(const std::string &name);
    virtual ~TestHelloTriangle();

    void onUpdate() override;
    void onRender() override;
    void onImGuiRender() override;

private:
    std::unique_ptr<IShader> m_shader;

    GLuint m_VAO; // Vertex Array Object
    GLuint m_VBO; // Vertex Buffer Object
    GLuint m_IBO; // Index Buffer Object, same as EBO (Elements Buffer Object)

    glm::vec4 m_triangleColor;
};