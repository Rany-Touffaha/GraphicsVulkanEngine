#pragma once

#include <filesystem>

namespace vulkanEng
{
    bool streq(gsl::czstring left, gsl::czstring right);
    std::vector<std::uint8_t> readFile(std::filesystem::path shader_path);
}
