#include "texture.hpp"
#include "renderer_backend/OpenGL/ogl_texture.hpp"

auto festGL::ITexture::createUnique(const std::filesystem::path &filename) -> std::unique_ptr<ITexture>
{
    return std::make_unique<festGL::OGLTexture>(filename);
}

auto festGL::ITexture::createUnique(const TextureInfo &specification) -> std::unique_ptr<ITexture>
{
    return std::make_unique<festGL::OGLTexture>(specification);
}

auto festGL::ITexture::createShared(const std::filesystem::path &filename) -> std::shared_ptr<ITexture>
{
    return std::make_shared<festGL::OGLTexture>(filename);
}

auto festGL::ITexture::createShared(const TextureInfo &specification) -> std::shared_ptr<ITexture>
{
    return std::make_shared<festGL::OGLTexture>(specification);
}

festGL::ITexture::ITexture(const TextureInfo &specification)
    : m_specification(specification)
{
}
