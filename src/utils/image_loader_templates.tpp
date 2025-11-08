#include "stb_image.h"

using namespace festGL;

static auto loadImage(const std::filesystem::path& filename, int& width, int& height, int& channels, bool isFlipped  = false) -> uint8_t* {
    stbi_set_flip_vertically_on_load(isFlipped);
    return stbi_load(filename.string().c_str(), &width, &height, &channels, 0);
}

template <ImageLoader::FlipType type>
constexpr auto ImageLoader::loadManaged(const std::filesystem::path &filename, 
    int& width, int& height, int& channels) -> ImageLoader::ImageDataContainer
{
    uint8_t* data = nullptr;

    if constexpr (type == ImageLoader::FlipType::Vertical) {
        data = loadImage(filename, width, height, channels, true);
    } else {
        data = loadImage(filename, width, height, channels);
    }

    return ImageLoader::ImageDataContainer(data);
}

template <ImageLoader::FlipType type>
constexpr auto ImageLoader::load(const std::filesystem::path &filename,
    int& width, int& height, int& channels) -> uint8_t *
{
    if constexpr (type == ImageLoader::FlipType::Vertical) {
        return loadImage(filename, width, height, channels, true);
    } else {
        return loadImage(filename, width, height, channels);
    }
}