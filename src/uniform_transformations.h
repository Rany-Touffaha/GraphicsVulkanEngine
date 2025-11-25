#pragma once
#include <glm/glm.hpp>

namespace vulkanEng
{
    struct UniformTransformations
    {
        glm::mat4 view;
        glm::mat4 projection;
    };
}
