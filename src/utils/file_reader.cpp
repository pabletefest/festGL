#include "file_reader.hpp"

#include <fstream>
#include <iterator>

std::string festGL::readFile(std::filesystem::path filepath)
{
    std::ifstream fileStream{ filepath, std::ios::binary };
    return { std::istreambuf_iterator{fileStream}, {} };
}