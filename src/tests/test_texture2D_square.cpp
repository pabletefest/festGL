#include "test_texture2D_square.hpp"

#include <print>
#include <filesystem>
#include <fstream>
#include <iterator>

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

static std::string readShaderFile(std::filesystem::path filepath) {
  std::ifstream fileStream{ filepath, std::ios::binary };
  return { std::istreambuf_iterator{fileStream}, {} };
}

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
    : Test(name), m_VAO(0), m_VBO(0), m_IBO(0), m_pipelineProgramID(0), m_textureID(0)
{
    m_pipelineProgramID= glCreateProgram();

    std::string vs = readShaderFile("./../../shaders/texture.glsl.vert");
    std::string fs = readShaderFile("./../../shaders/texture.glsl.frag");

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_quadPositions), g_quadPositions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void *)8);

    glGenBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_quadIndexes), g_quadIndexes, GL_STATIC_DRAW);

    stbi_set_flip_vertically_on_load(true);
    int width, height, channels;
    unsigned char *imageData = stbi_load("./../../resources/red-brick-wall-2048x2048.jpg",&width, &height, &channels, 0);

    if (!imageData) {
        std::println("Couldn't load image data!");
        exit(1);
    }

    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindVertexArray(0);
    stbi_image_free(imageData);
}

TestText2DSquare::~TestText2DSquare()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteProgram(m_pipelineProgramID);
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

    glUseProgram(m_pipelineProgramID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glBindVertexArray(m_VAO);

    GLint location = glGetUniformLocation(m_pipelineProgramID, "uTextCoord");
    glUniform1i(location, 0);

    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void TestText2DSquare::onImGuiRender()
{
}
