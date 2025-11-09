#pragma once

#include <string>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <vector>

namespace festGL {
    auto readFileAsString(std::filesystem::path filepath) -> std::string;

    template<typename T>
    auto readFile(std::filesystem::path filepath) ->std::vector<T>
    {
        std::basic_ifstream<T> fileStream{ filepath, std::ios::binary };
        return { std::istreambuf_iterator<T>{fileStream}, {} };
    }
};