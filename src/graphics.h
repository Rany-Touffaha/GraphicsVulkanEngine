#pragma once

#include <vulkan/vulkan.h>
#include <glfw_window.h>
#include <vertex.h>
#include <buffer_handle.h>

namespace vulkanEng
{
    class Graphics final
    {
    public:
        Graphics(gsl::not_null<Window*> window);
        ~Graphics();

        bool BeginFrame();
        void RenderBuffer(BufferHandle handle, std::uint32_t vertex_count);   
        void EndFrame();

        BufferHandle createVertexBuffer(gsl::span<Vertex> vertices);
        void destroyVertexBuffer(BufferHandle handle);

    private:

        struct QueueFamilyIndices
        {
            std::optional<std::uint32_t> graphics_family = std::nullopt;
            std::optional<std::uint32_t> presentation_family = std::nullopt;

            bool IsValid() const
            {
                return graphics_family.has_value() && presentation_family.has_value();
            }
        };

        struct SwapChainProperties
        {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> present_modes;

            bool IsValid() const
            {
                return !formats.empty() && !present_modes.empty();
            }
        };
        
        // Initialization
        void initializeVulkan();
        void createInstance();
        void setupDebugMessenger();
        void pickPhysicalDevice();
        void createLogicalDeviceAndQueues();
        void createSurface();
        void createSwapChain();
        void createImageViews();
        void createRenderPass();
        void createGraphicsPipeline();
        void createFramebuffers();
        void createCommandPool();
        void createCommandBuffer();
        void createSignals();

        void RecreateSwapChain();
        void CleanupSwapChain();

        // Rendering
        void BeginCommands();
        void EndCommands();


        std::vector<gsl::czstring> getRequiredInstanceExtensions();

        static gsl::span<gsl::czstring> getSuggestedInstanceExtensions();
        static std::vector<VkExtensionProperties> getSupportedInstanceExtensions();
        static bool areAllExtensionsSupported(gsl::span<gsl::czstring> extensions);

        static std::vector<VkLayerProperties> getSupportedValidationLayers();
        bool areAllLayersSupported(gsl::span<gsl::czstring> layers);

        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        SwapChainProperties getSwapChainProperties(VkPhysicalDevice device);
        bool IsDeviceSuitable(VkPhysicalDevice device);
        std::vector<VkPhysicalDevice> getAvailableDevices();
        bool AreAllDeviceExtensionsSupported(VkPhysicalDevice device);
        std::vector<VkExtensionProperties> getDeviceAvailableExtensions(VkPhysicalDevice device);

        VkSurfaceFormatKHR chooseSwapSurfaceFormat(gsl::span<VkSurfaceFormatKHR> formats);
        VkPresentModeKHR chooseSwapPresentMode(gsl::span<VkPresentModeKHR> modes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        std::uint32_t chooseSwapImageCount(const VkSurfaceCapabilitiesKHR& capabilities);

        VkShaderModule createShaderModule(gsl::span<std::uint8_t> buffer);

        std::uint32_t findMemoryType(std::uint32_t type_bits_filter,
            VkMemoryPropertyFlags required_properties);

        VkViewport getViewport();
        VkRect2D getScissor();

        std::array<gsl::czstring, 2> required_device_extensions_ = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
            "VK_KHR_portability_subset"
        };

        VkInstance instance_ = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT debug_messenger_;

        VkPhysicalDevice physical_device_ = VK_NULL_HANDLE;
        VkDevice logical_device_ = VK_NULL_HANDLE;
        VkQueue graphics_queue_ = VK_NULL_HANDLE;
        VkQueue present_queue_ = VK_NULL_HANDLE;

        VkSurfaceKHR surface_ = VK_NULL_HANDLE;
        VkSwapchainKHR swap_chain_ = VK_NULL_HANDLE;
        VkSurfaceFormatKHR surface_format_;
        VkPresentModeKHR present_mode_;
        VkExtent2D extent_;

        std::vector<VkImage> swap_chain_images_;
        std::vector<VkImageView> swap_chain_image_views_;
        std::vector<VkFramebuffer> swap_chain_framebuffers_;

        VkPipelineLayout pipeline_layout_ = VK_NULL_HANDLE;
        VkRenderPass render_pass_ = VK_NULL_HANDLE;
        VkPipeline pipeline_ = VK_NULL_HANDLE;

        VkCommandPool command_pool_ = VK_NULL_HANDLE;
        VkCommandBuffer command_buffer_ = VK_NULL_HANDLE;

        VkSemaphore image_available_signal_ = VK_NULL_HANDLE;
        VkSemaphore render_finished_signal_ = VK_NULL_HANDLE;
        VkFence still_rendering_fence_ = VK_NULL_HANDLE;

        std::uint32_t current_image_index_ = 0;

        gsl::not_null<Window*> window_;
        bool validation_enabled_ = true;
    };
}
