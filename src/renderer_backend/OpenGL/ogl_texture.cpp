#include "ogl_texture.hpp"
#include "utils/image_loader.hpp"

#include <Glad/gl.h>

#include <print>
#include <cassert>

struct GLTextureFormat {
    GLenum internalFormat;
    GLenum format;
};

static constexpr auto getGLFormat(festGL::TextureFormat format) -> GLTextureFormat {
    switch(format) {
        case festGL::TextureFormat::RGB8:
            return { GL_RGB8, GL_RGB };
        case festGL::TextureFormat::RGBA8:
            return { GL_RGBA8, GL_RGBA };
        default:
            assert(false && "Invalid texture format!");
    }

    return {};
}

festGL::OGLTexture::OGLTexture(const std::filesystem::path &filename)
    : ITexture({})
{
    int width, height, channels;
    auto imageData = ImageLoader::loadManaged<ImageLoader::FlipType::Vertical>(filename, width, height, channels);

    if (!imageData) {
        std::println("Couldn't load image data!");
        exit(1);
    }

    m_specification.width = width;
    m_specification.height = height;
    m_specification.useMipmaps = true;

    switch(channels) {
        case 3:
            m_specification.format = festGL::TextureFormat::RGB8;
            break;
            
        case 4:
            m_specification.format = festGL::TextureFormat::RGBA8;
            break;
        
        default:
            assert(false && "Invalid texture format!");
    }

    createTexture();
    updateTexture(imageData.get());
}

festGL::OGLTexture::OGLTexture(const festGL::TextureInfo &specification)
    : ITexture(specification)
{
    createTexture();
}

festGL::OGLTexture::~OGLTexture()
{
    glBindTextureUnit(0, 0);
    glDeleteTextures(1, &m_textureID);
}

auto festGL::OGLTexture::apply(uint32_t textureSlot) -> void
{
    glBindTextureUnit(textureSlot, m_textureID);
}

auto festGL::OGLTexture::setData(const uint8_t *const buffer) -> void
{
    updateTexture(buffer);

    if (m_specification.useMipmaps) {
        generateMipmap();
    }
}

auto festGL::OGLTexture::setData(std::span<uint8_t> buffer) -> void
{
    updateTexture(buffer.data());

    if (m_specification.useMipmaps) {
        generateMipmap();
    }
}

auto festGL::OGLTexture::createTexture() -> void
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_textureID);
  
    glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(m_textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(m_textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    auto [internalFormat, _] = getGLFormat(m_specification.format);
    
    glTextureStorage2D(m_textureID, 1, internalFormat, m_specification.width, m_specification.height);
}

auto festGL::OGLTexture::updateTexture(const uint8_t *const data) -> void
{
    auto [_, format] = getGLFormat(m_specification.format);
    glTextureSubImage2D(m_textureID, 0, 0, 0, m_specification.width, m_specification.height, format, GL_UNSIGNED_BYTE, data);
}

auto festGL::OGLTexture::generateMipmap() -> void
{
    glGenerateTextureMipmap(m_textureID);
}
