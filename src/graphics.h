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
        std::vector<gsl::czstring> getRequiredInstanceExtensions();

        static gsl::span<gsl::czstring> getSuggestedInstanceExtensions();
        static std::vector<VkExtensionProperties> getSupportedInstanceExtensions();
        static bool areAllExtensionsSupported(gsl::span<gsl::czstring> extensions);

        static std::vector<VkLayerProperties> getSupportedValidationLayers();
        bool areAllLayersSupported(gsl::span<gsl::czstring> layers);

        VkInstance instance_ = nullptr;
        gsl::not_null<Window*> window_;
        bool validation_enabled_ = true;
    };
}
