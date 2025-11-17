#pragma once

#include "test.hpp"
#include "renderer/shader.hpp"

#include <glad/gl.h>

#include <memory>

class TestRotating3DCube : public Test {
public:
    TestRotating3DCube(const std::string &name);
    virtual ~TestRotating3DCube();

    void onUpdate() override;
    void onRender() override;
    void onImGuiRender() override;

private:
    std::unique_ptr<festGL::IShader> m_shader;

    GLuint m_VAO; // Vertex Array Object
    GLuint m_VBO; // Vertex Buffer Object
};