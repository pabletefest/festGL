#include "image_loader.hpp"

#include "stb_image.h"

#include <stdio.h>

auto festGL::ImageLoader::StbDataDeleter::operator()(unsigned char *data) -> void
{
    stbi_image_free(data);
}

auto festGL::ImageLoader::release(uint8_t *data) -> void
{
    stbi_image_free(data);
}
