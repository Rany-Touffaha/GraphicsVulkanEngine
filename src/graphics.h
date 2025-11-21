#pragma once

#include <vulkan/vulkan.h>
#include <glfw_window.h>

namespace vulkanEng
{
    class Graphics final
    {
    public:
        Graphics(gsl::not_null<Window*> window);
        ~Graphics();

    private:
        void initializeVulkan();
        void createInstance();
        gsl::span<gsl::czstring> getSuggestedExtensions();

        VkInstance instance_ = nullptr;
        gsl::not_null<Window*> window_;
    };
}
