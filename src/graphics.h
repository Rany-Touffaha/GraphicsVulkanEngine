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

        struct QueueFamilyIndices
        {
            std::optional<std::uint32_t> graphics_family = std::nullopt;
            std::optional<std::uint32_t> presentation_family = std::nullopt;

            bool IsValid() const
            {
                return graphics_family.has_value();
            }
        };
        
        void initializeVulkan();
        void createInstance();
        void setupDebugMessenger();
        void pickPhysicalDevice();
        void createLogicalDeviceAndQueues();
        std::vector<gsl::czstring> getRequiredInstanceExtensions();

        static gsl::span<gsl::czstring> getSuggestedInstanceExtensions();
        static std::vector<VkExtensionProperties> getSupportedInstanceExtensions();
        static bool areAllExtensionsSupported(gsl::span<gsl::czstring> extensions);

        static std::vector<VkLayerProperties> getSupportedValidationLayers();
        bool areAllLayersSupported(gsl::span<gsl::czstring> layers);

        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        bool IsDeviceSuitable(VkPhysicalDevice device);
        std::vector<VkPhysicalDevice> getAvailableDevices();

        VkInstance instance_ = nullptr;
        VkPhysicalDevice physical_device_ = nullptr;
        VkDevice logical_device_ = nullptr;
        VkDebugUtilsMessengerEXT debug_messenger_;
        gsl::not_null<Window*> window_;
        bool validation_enabled_ = true;
    };
}
