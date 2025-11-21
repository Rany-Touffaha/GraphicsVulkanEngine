#include <precomp.h>
#include <graphics.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vulkan/vulkan.h>

namespace vulkanEng
{
    Graphics::Graphics(gsl::not_null<Window*> window)
        : window_(window)
    {
        initializeVulkan();
    }

    Graphics::~Graphics()
    {
        if(instance_ != nullptr) {
            vkDestroyInstance(instance_, nullptr);
        }
    }

    void Graphics::initializeVulkan()
    {
        createInstance();
    }

    void Graphics::createInstance()
    #ifndef VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR
    #define VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR 0x00000001
    #endif
    {
        gsl::span<gsl::czstring> suggested_extensions = getSuggestedExtensions();
        std::vector<const char*> extensions(suggested_extensions.begin(), suggested_extensions.end());
        bool found_portability = false;
        for (const char* ext : extensions) {
            if (strcmp(ext, "VK_KHR_portability_enumeration") == 0) {
                found_portability = true;
                break;
            }
        }
        if (!found_portability) {
            extensions.push_back("VK_KHR_portability_enumeration");
        }

        VkApplicationInfo app_info = {};
        app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app_info.pNext = nullptr;
        app_info.pApplicationName = "Vulkan Engine";
        app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.pEngineName = "VEngine";
        app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo instance_creation_info = {};
        instance_creation_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instance_creation_info.pNext = nullptr;
        instance_creation_info.pApplicationInfo = &app_info;
        instance_creation_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        instance_creation_info.ppEnabledExtensionNames = extensions.data();
        instance_creation_info.enabledLayerCount = 0;
        instance_creation_info.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

        VkResult result = vkCreateInstance(&instance_creation_info, nullptr, &instance_);
        if (result != VK_SUCCESS)
        {
            std::cerr << "Vulkan instance creation failed: " << result << std::endl;
            std::exit(EXIT_FAILURE);
        }

        std::cout << "Vulkan instance created successfully" << std::endl;
    }

    gsl::span<gsl::czstring> Graphics::getSuggestedExtensions()
    {
        uint32_t glfw_extension_count = 0;
        gsl::czstring* glfw_extensions;
        glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
        return {glfw_extensions, glfw_extension_count};
    }
}
