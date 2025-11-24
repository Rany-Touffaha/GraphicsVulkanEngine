#pragma once

#include <vulkan/vulkan.h>

namespace vulkanEng
{
    struct BufferHandle
    {
        VkBuffer buffer;
        VkDeviceMemory memory;
    };
}
