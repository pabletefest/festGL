#include "test_texture2D_square.hpp"
#include "image_loader.hpp"

#include <print>
#include <filesystem>

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

using namespace festGL;

static const GLfloat g_quadPositions[] = {
   -0.5f,  0.5f, 0.0f, 1.0f, // Top left
    0.5f,  0.5f, 1.0f, 1.0f, // Top right
    0.5f, -0.5f, 1.0f, 0.0f, // Bottom right
   -0.5f, -0.5f, 0.0f, 0.0f  // Bottom left
};

static const GLuint g_quadIndexes[] = {
    0, 1, 2,
    2, 3, 0
};

TestText2DSquare::TestText2DSquare(const std::string &name)
    : Test(name), m_VAO(0), m_VBO(0), m_IBO(0), m_textureID(0)
{
    std::filesystem::path vsPath("./../../shaders/texture.glsl.vert");
    std::filesystem::path fsPath("./../../shaders/texture.glsl.frag");

    m_shader = IShader::createUnique(vsPath, fsPath);

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_quadPositions), g_quadPositions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void *)8);

    glGenBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_quadIndexes), g_quadIndexes, GL_STATIC_DRAW);

    int width, height, channels;
    std::filesystem::path imageFilename = "./../../resources/red-brick-wall-2048x2048.jpg";
    auto imageData = ImageLoader::loadManaged<ImageLoader::FlipType::Vertical>(imageFilename, 
        width, height, channels);

    if (!imageData) {
        std::println("Couldn't load image data!");
        exit(1);
    }

    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData.get());
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindVertexArray(0);
}

TestText2DSquare::~TestText2DSquare()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
}

void TestText2DSquare::onUpdate()
{
}

void TestText2DSquare::onRender()
{
    glClear(GL_COLOR_BUFFER_BIT);

    m_shader->apply();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glBindVertexArray(m_VAO);

    m_shader->setData("uTextCoord", 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    // glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void TestText2DSquare::onImGuiRender()
{
}
