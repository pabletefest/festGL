#pragma once

#include "renderer/shader.hpp"

#include "Glad/gl.h"

namespace festGL {
    class OGLShader : public IShader {
        public:
            OGLShader(const std::filesystem::path& filename);
            OGLShader(const std::filesystem::path& vertShaderPath, const std::filesystem::path fragShaderPath);
            ~OGLShader() override;

            auto apply() -> void override;

        private:
            auto compileShader(const std::string& source, GLenum type) -> GLuint;
    };
};
