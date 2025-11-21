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

        static gsl::span<gsl::czstring> getSuggestedInstanceExtensions();
        static std::vector<VkExtensionProperties> getSupportedInstanceExtensions();

        VkInstance instance_ = nullptr;
        gsl::not_null<Window*> window_;
    };
}
