#pragma once

#include <vulkan/vulkan.h>

namespace vulkanEng
{
    struct BufferHandle
    {
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
    };
}
