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
    : Test(name), m_VAO(0), m_VBO(0), m_IBO(0), m_textureID(0), m_textureColor(glm::vec4(1.0f))
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
    
    std::filesystem::path imageFilename = "./../../resources/red-brick-wall-2048x2048.jpg";
    
    // m_texture = ITexture::createUnique(imageFilename);
    
    int width, height, channels;
    auto imageData = ImageLoader::loadManaged<ImageLoader::FlipType::Vertical>(imageFilename, 
        width, height, channels);

    if (!imageData) {
        std::println("Couldn't load image data!");
        exit(1);
    }

    festGL::TextureInfo textureSpecification;

    textureSpecification.width = width;
    textureSpecification.height = height;
    textureSpecification.useMipmaps = true;

    switch(channels) {
        case 3:
            textureSpecification.format = festGL::TextureFormat::RGB8;
            break;
            
        case 4:
            textureSpecification.format = festGL::TextureFormat::RGBA8;
            break;
        
        default:
            assert(false && "Invalid texture format!");
    }

    m_texture = ITexture::createUnique(textureSpecification);
    m_texture->setData(imageData.get());

    glBindVertexArray(0);
}

TestText2DSquare::~TestText2DSquare()
{
    glBindVertexArray(0);

    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_IBO);
    glDeleteVertexArrays(1, &m_VAO);
}

void TestText2DSquare::onUpdate()
{
}

void TestText2DSquare::onRender()
{
    glClear(GL_COLOR_BUFFER_BIT);

    constexpr uint32_t textureSlot = 0;

    m_shader->apply();
    m_texture->apply(textureSlot);
    glBindVertexArray(m_VAO);

    m_shader->setData("uWallTexture", textureSlot);
    m_shader->setDataBuffer<glm::vec4>("uColor", { m_textureColor } );

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    // glUseProgram(0);
    glBindTextureUnit(0, 0);
    glBindVertexArray(0);
}

void TestText2DSquare::onImGuiRender()
{
    ImGui::ColorEdit4("Texture color", glm::value_ptr(m_textureColor));
}
