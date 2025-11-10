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

    glCreateVertexArrays(1, &m_VAO);

    glCreateBuffers(1, &m_VBO);
    glNamedBufferStorage(m_VBO, sizeof(g_quadPositions), g_quadPositions, GL_DYNAMIC_STORAGE_BIT);
    
    glCreateBuffers(1, &m_IBO);
    glNamedBufferStorage(m_IBO, sizeof(g_quadIndexes), g_quadIndexes, GL_DYNAMIC_STORAGE_BIT);
    
    glVertexArrayVertexBuffer(m_VAO, 0, m_VBO, 0, 4 * sizeof(float));
    glVertexArrayElementBuffer(m_VAO, m_IBO);

    glEnableVertexArrayAttrib(m_VAO, 0);
    glEnableVertexArrayAttrib(m_VAO, 1);

    glVertexArrayAttribFormat(m_VAO, 0, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(m_VAO, 1, 2, GL_FLOAT, GL_FALSE, 8);

    glVertexArrayAttribBinding(m_VAO, 0, 0);
    glVertexArrayAttribBinding(m_VAO, 1, 0);

    int width, height, channels;
    std::filesystem::path imageFilename = "./../../resources/red-brick-wall-2048x2048.jpg";
    auto imageData = ImageLoader::loadManaged<ImageLoader::FlipType::Vertical>(imageFilename, 
        width, height, channels);

    if (!imageData) {
        std::println("Couldn't load image data!");
        exit(1);
    }

    GLenum internalFormat = GL_NONE;
    GLenum format = GL_NONE;

    switch(channels) {
        case 3:
            internalFormat = GL_RGB8;
            format = GL_RGB;
            break;
        
        case 4:
            internalFormat = GL_RGBA8;
            format = GL_RGBA;
            break;
        
        default:
            assert(false && "Invalid texture format!");
    }

    glCreateTextures(GL_TEXTURE_2D, 1, &m_textureID);
  
    glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(m_textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(m_textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTextureStorage2D(m_textureID, 1, internalFormat, width, height);
    glTextureSubImage2D(m_textureID, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, imageData.get());
    glGenerateTextureMipmap(m_textureID);

    glBindVertexArray(0);
}

TestText2DSquare::~TestText2DSquare()
{
    glBindVertexArray(0);
    glBindTextureUnit(0, 0);

    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteTextures(1, &m_textureID);
}

void TestText2DSquare::onUpdate()
{
}

void TestText2DSquare::onRender()
{
    glClear(GL_COLOR_BUFFER_BIT);

    m_shader->apply();
    glBindTextureUnit(0, m_textureID);
    glBindVertexArray(m_VAO);

    m_shader->setData("uTextCoord", 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    // glUseProgram(0);
    glBindTextureUnit(0, 0);
    glBindVertexArray(0);
}

void TestText2DSquare::onImGuiRender()
{
}
