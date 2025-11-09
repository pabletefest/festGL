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

        // protected:
        //     template<typename T>
        //     struct OGLShaderDataSetter : public IShaderDataSetter<T>  {
        //         void operator()(uint32_t id, const std::string& name, const T& data) override;
        //         void operator()(uint32_t id, const std::string& name, std::span<T> buffer) override;
        //     };

        private:
            auto compileShader(const std::string& source, GLenum type) -> GLuint;
    };
};
