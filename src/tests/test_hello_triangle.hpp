#pragma once

#include "test.hpp"

#include <glad/gl.h>

#include <glm/vec4.hpp>

class TestHelloTriangle : public Test {
public:
    TestHelloTriangle(const std::string &name);
    virtual ~TestHelloTriangle();

    void onUpdate() override;
    void onRender() override;
    void onImGuiRender() override;

private:
    GLuint m_pipelineProgramID; // Shaders program ID

    GLuint m_VAO; // Vertex Array Object
    GLuint m_VBO; // Vertex Buffer Object
    GLuint m_IBO; // Index Buffer Object, same as EBO (Elements Buffer Object)

    glm::vec4 m_triangleColor;
};