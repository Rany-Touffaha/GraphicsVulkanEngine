#include <precomp.h>
#include <utilities.h>
#include <fstream>
#include <spdlog/spdlog.h>

namespace vulkanEng
{
    bool streq(gsl::czstring left, gsl::czstring right)
    {
        return std::strcmp(left, right) == 0;
    }

    std::vector<std::uint8_t> readFile(std::filesystem::path shader_path)
    {
        if (!std::filesystem::exists(shader_path)) {
            spdlog::error("Shader file does not exist: {}", shader_path.string());
            return {};
        }

        if(!std::filesystem::is_regular_file(shader_path)) {
            spdlog::error("Shader path is not a regular file: {}", shader_path.string());
            return {};
        }

        std::ifstream file(shader_path, std::ios::binary);
        if (!file.is_open()) {
            spdlog::error("Failed to open shader file: {}", shader_path.string());
            return {};
        }
        
        const std::uint32_t size = std::filesystem::file_size(shader_path);

        std::vector<std::uint8_t> buffer(size);
        file.read(reinterpret_cast<char*>(buffer.data()), size);
        if (!file) {
            spdlog::error("Failed to read shader file: {}", shader_path.string());
            return {};
        }
        return buffer;
    }
}

