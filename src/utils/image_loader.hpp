#pragma once

#include <filesystem>
#include <memory>

namespace festGL {
    class ImageLoader {
        public:
            struct StbDataDeleter {
                auto operator()(unsigned char* data) -> void;
            };
        
            using ImageDataContainer = std::unique_ptr<uint8_t, StbDataDeleter>;
            
            enum class FlipType {
                NoFlip,
                Vertical
            };

            template<FlipType type>
            static constexpr auto loadManaged(const std::filesystem::path& filename, 
                int& width, int& height, int& channels) -> ImageDataContainer;
            
            template<FlipType type>
            static constexpr auto load(const std::filesystem::path& filename, 
                int& width, int& height, int& channels) -> uint8_t*;

            static auto release(uint8_t* data) -> void;
    };

    #include "image_loader_templates.tpp"
};
