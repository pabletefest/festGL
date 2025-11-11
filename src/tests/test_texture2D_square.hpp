#pragma once

#include "test.hpp"
#include "renderer/shader.hpp"
#include "renderer/texture.hpp"

#include <glad/gl.h>
#include <glm/vec4.hpp>

#include <memory>

class TestText2DSquare : public Test {
public:
    TestText2DSquare(const std::string &name);
    virtual ~TestText2DSquare();

    void onUpdate() override;
    void onRender() override;
    void onImGuiRender() override;

private:
    std::unique_ptr<festGL::IShader> m_shader;
    std::unique_ptr<festGL::ITexture> m_texture;

    GLuint m_VAO; // Vertex Array Object
    GLuint m_VBO; // Vertex Buffer Object
    GLuint m_IBO; // Index Buffer Object, same as EBO (Elements Buffer Object)
    GLuint m_textureID;
};