#pragma once

#include "renderer/texture.hpp"

namespace festGL {
    class OGLTexture : public ITexture {
        public:
            OGLTexture(const std::filesystem::path& filename);
            OGLTexture(const TextureInfo& specification);
            ~OGLTexture() override;

            auto apply(uint32_t textureSlot) -> void override;

            auto setData(const uint8_t *const buffer) -> void override;
            auto setData(std::span<uint8_t> buffer) -> void override;

        private:
            auto createTexture() -> void;
            auto updateTexture(const uint8_t *const data) -> void;
            auto generateMipmap() -> void;
    };
};
