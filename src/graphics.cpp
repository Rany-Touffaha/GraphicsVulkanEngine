#include <precomp.h>
#include <graphics.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vulkan/vulkan.h>
#include <spdlog/spdlog.h>
#include <set>

#pragma region VK_FUNCTION_EXT_IMPL

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(
    VkInstance                                  instance,
    const VkDebugUtilsMessengerCreateInfoEXT*   info,
    const VkAllocationCallbacks*                allocator,
    VkDebugUtilsMessengerEXT*                   debug_messenger)
{
    PFN_vkCreateDebugUtilsMessengerEXT function = 
        reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
    
    if (function != nullptr) {
        return function(instance, info, allocator, debug_messenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(
    VkInstance                                  instance,
    VkDebugUtilsMessengerEXT                    debug_messenger,
    const VkAllocationCallbacks*                allocator)
{
    PFN_vkDestroyDebugUtilsMessengerEXT function =
        reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
    
    if (function != nullptr) {
        function(instance, debug_messenger, allocator);
    }
}

#pragma endregion

namespace vulkanEng
{
    #pragma region VALIDATION_LAYERS

    static VKAPI_ATTR VkBool32 VKAPI_CALL ValidationCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT       severity,
        VkDebugUtilsMessageTypeFlagsEXT              type,
        const VkDebugUtilsMessengerCallbackDataEXT*  callback_data,
        void*                                        user_data)
    {
        if(severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT){
            spdlog::warn("Vulkan Validation: {}", callback_data->pMessage);
        } else {
            spdlog::error("Vulkan Error: {}", callback_data->pMessage);
        }
        return VK_FALSE;
    }

    static VkDebugUtilsMessengerCreateInfoEXT GetCreateMessengerInfo(){
        VkDebugUtilsMessengerCreateInfoEXT creation_info = {};
        creation_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        creation_info.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        creation_info.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
        creation_info.pfnUserCallback = ValidationCallback;
        creation_info.pUserData = nullptr;
        return creation_info;
    }

    std::vector<VkLayerProperties> Graphics::getSupportedValidationLayers()
    {
        uint32_t count;
        vkEnumerateInstanceLayerProperties(&count, nullptr);

        if(count == 0) {
            return {};
        }

        std::vector<VkLayerProperties> properties(count);
        vkEnumerateInstanceLayerProperties(&count, properties.data());
        return properties;
    }

    bool LayerMatchesName(gsl::czstring name, const VkLayerProperties& property)
    {
        return vulkanEng::streq(property.layerName, name);
    }

    bool IsLayerSupported(gsl::span<VkLayerProperties> layers, gsl::czstring name)
    {
        return std::any_of(layers.begin(), layers.end(),
                           std::bind_front(LayerMatchesName, name));
    }

    bool Graphics::areAllLayersSupported(gsl::span<gsl::czstring> layers)
    {
        std::vector<VkLayerProperties> supported_layers = getSupportedValidationLayers();

        return std::all_of(layers.begin(), layers.end(),
            [&](gsl::czstring name) { return IsLayerSupported(supported_layers, name); });
    }

    void Graphics::setupDebugMessenger()
    {
        if (!validation_enabled_) {
            return;
        }

        VkDebugUtilsMessengerCreateInfoEXT info = GetCreateMessengerInfo();

        VkResult result = vkCreateDebugUtilsMessengerEXT(
            instance_,
            &info,
            nullptr,
            &debug_messenger_);
        
        if (result != VK_SUCCESS) {
            spdlog::error("Failed to set up debug messenger: {}", static_cast<int>(result));
            return;
        }
    }

    #pragma endregion
 
    #pragma region INSTANCE_AND_EXTENSIONS

    void Graphics::createInstance()
    #ifndef VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR
    #define VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR 0x00000001
    #endif
    {
        std::array<gsl::czstring, 1> validation_layers = {
            "VK_LAYER_KHRONOS_validation"
        };

        if(!areAllLayersSupported(validation_layers))
        {
            validation_enabled_ = false;
        }


        std::vector<gsl::czstring> required_extensions = getRequiredInstanceExtensions();
        
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
        instance_creation_info.enabledExtensionCount = static_cast<uint32_t>(required_extensions.size());
        instance_creation_info.ppEnabledExtensionNames = required_extensions.data();
        instance_creation_info.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

        VkDebugUtilsMessengerCreateInfoEXT messenger_creation_info = GetCreateMessengerInfo();

        if (validation_enabled_)
        {
            instance_creation_info.pNext = &messenger_creation_info;
            instance_creation_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
            instance_creation_info.ppEnabledLayerNames = validation_layers.data();
        }
        else
        {
            instance_creation_info.enabledLayerCount = 0;
            instance_creation_info.ppEnabledLayerNames = nullptr;
        }

        VkResult result = vkCreateInstance(&instance_creation_info, nullptr, &instance_);
        if (result != VK_SUCCESS)
        {
            std::cerr << "Vulkan instance creation failed: " << result << std::endl;
            std::exit(EXIT_FAILURE);
        }

        std::cout << "Vulkan instance created successfully" << std::endl;
    }

    gsl::span<gsl::czstring> Graphics::getSuggestedInstanceExtensions()
    {
        uint32_t glfw_extension_count = 0;
        gsl::czstring* glfw_extensions;
        glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
        return {glfw_extensions, glfw_extension_count};
    }

    std::vector<gsl::czstring> Graphics::getRequiredInstanceExtensions()
    {
        gsl::span<gsl::czstring> suggested_extensions = getSuggestedInstanceExtensions();
        std::vector<gsl::czstring> required_extensions(suggested_extensions.begin(), suggested_extensions.end());

        // Add debug utils extension if validation is enabled
        if (validation_enabled_)
        {
            required_extensions.push_back("VK_EXT_debug_utils");
        }

        // Ensure VK_KHR_portability_enumeration is present
        bool found_portability = false;
        for (const auto& ext : required_extensions) {
            if (strcmp(ext, "VK_KHR_portability_enumeration") == 0) {
                found_portability = true;
                break;
            }
        }
        if (!found_portability) {
            required_extensions.push_back("VK_KHR_portability_enumeration");
        }

        if (!areAllExtensionsSupported(required_extensions)) {
            std::exit(EXIT_FAILURE);
        }

        return required_extensions;
    }

    std::vector<VkExtensionProperties> Graphics::getSupportedInstanceExtensions()
    {
        uint32_t count;
        vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);

        if(count == 0) {
            return {};
        }

        std::vector<VkExtensionProperties> properties(count);
        vkEnumerateInstanceExtensionProperties(nullptr, &count, properties.data());
        return properties;
    }

    bool ExtensionMatchesName(gsl::czstring name, const VkExtensionProperties& property)
    {
        return vulkanEng::streq(property.extensionName, name);
    }

    bool IsExtensionSupported(gsl::span<VkExtensionProperties> extensions, gsl::czstring name)
    {
        return std::any_of(extensions.begin(), extensions.end(),
                           std::bind_front(ExtensionMatchesName, name));
    }

    bool Graphics::areAllExtensionsSupported(gsl::span<gsl::czstring> extensions)
    {
        std::vector<VkExtensionProperties> supported_extensions = getSupportedInstanceExtensions();

        return std::all_of(extensions.begin(), extensions.end(), 
            std::bind_front(IsExtensionSupported, supported_extensions));
    }

    #pragma endregion

    #pragma region DEVICES_AND_QUEUES

    Graphics::QueueFamilyIndices Graphics::findQueueFamilies(VkPhysicalDevice device)
    {
        uint32_t queue_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

        std::vector<VkQueueFamilyProperties> families(queue_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, families.data());

        auto graphics_family_it =
            std::find_if(families.begin(), families.end(),
                [&](const VkQueueFamilyProperties& props) {
                    return props.queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT);
                });
        
        QueueFamilyIndices result;
        result.graphics_family = graphics_family_it - families.begin();

        for (std::uint32_t i = 0; i < families.size(); i++)
        {
            VkBool32 has_presentation_support = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(
                device,
                i,
                surface_,
                &has_presentation_support);

            if (has_presentation_support) {
                result.presentation_family = i;
                break;
            }
        }

        return result;
    }

    bool Graphics::IsDeviceSuitable(VkPhysicalDevice device)
    {
        QueueFamilyIndices families = findQueueFamilies(device);

        return families.IsValid();
    }

    void Graphics::pickPhysicalDevice()
    {
        std::vector<VkPhysicalDevice> devices = getAvailableDevices();

        std::erase_if(devices, std::not_fn(std::bind_front(&Graphics::IsDeviceSuitable, this)));

        if(devices.empty()) {
            spdlog::error("No suitable GPU found that matches the criteria.");
            std::exit(EXIT_FAILURE);
        }
        
        physical_device_ = devices[0];
    }

    std::vector<VkPhysicalDevice> Graphics::getAvailableDevices()
    {
        uint32_t device_count = 0;
        vkEnumeratePhysicalDevices(instance_, &device_count, nullptr);

        if (device_count == 0) {
            spdlog::error("Failed to find GPUs with Vulkan support.");
            return {};
        }

        std::vector<VkPhysicalDevice> devices(device_count);
        vkEnumeratePhysicalDevices(instance_, &device_count, devices.data());
        return devices;
    }

    void Graphics::createLogicalDeviceAndQueues()
    {
        QueueFamilyIndices picked_device_families = findQueueFamilies(physical_device_);

        if(!picked_device_families.IsValid()) {
            std::exit(EXIT_SUCCESS);
        }

        std::set<std::uint32_t> unique_queue_families = {
            picked_device_families.graphics_family.value(),
            picked_device_families.presentation_family.value()
        };

        std::float_t queue_priority = 1.0f;

        std::vector<VkDeviceQueueCreateInfo> queue_create_infos;

        for(std::uint32_t unique_queue_family : unique_queue_families)
        {
            VkDeviceQueueCreateInfo queue_info = {};
            queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_info.queueFamilyIndex = unique_queue_family;
            queue_info.queueCount = 1;
            queue_info.pQueuePriorities = &queue_priority;
            queue_create_infos.push_back(queue_info);
        }

        VkPhysicalDeviceFeatures required_features = {};

        VkDeviceCreateInfo device_info = {};
        device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        device_info.queueCreateInfoCount = queue_create_infos.size();
        device_info.pQueueCreateInfos = queue_create_infos.data();
        device_info.enabledExtensionCount = 0;
        device_info.enabledLayerCount = 0;

        VkResult result = vkCreateDevice(
            physical_device_,
            &device_info,
            nullptr,
            &logical_device_
        );

        if (result != VK_SUCCESS) {
            spdlog::error("Failed to create logical device: {}", static_cast<int>(result));
            std::exit(EXIT_FAILURE);
        }

        vkGetDeviceQueue(
            logical_device_,
            picked_device_families.graphics_family.value(),
            0,
            &graphics_queue_
        );

        vkGetDeviceQueue(
            logical_device_,
            picked_device_families.presentation_family.value(),
            0,
            &present_queue_
        );
    }

    #pragma endregion

    #pragma region PRESENTATION

    void Graphics::createSurface()
    {
        VkResult result = glfwCreateWindowSurface(
            instance_,
            window_->getHandle(),
            nullptr,
            &surface_);

        if (result != VK_SUCCESS) {
            std::exit(EXIT_FAILURE);
        }
    }


    #pragma endregion

    Graphics::Graphics(gsl::not_null<Window*> window)
        : window_(window)
    {
        #if !defined(NDEBUG)
            validation_enabled_ = true;
        #endif

        initializeVulkan();
    }

    Graphics::~Graphics()
    {
        if(logical_device_ != VK_NULL_HANDLE) {
            vkDestroyDevice(logical_device_, nullptr);
        }

        if(instance_ != VK_NULL_HANDLE) {
            if(surface_ != VK_NULL_HANDLE) {
                vkDestroySurfaceKHR(instance_, surface_, nullptr);
            }


            if(debug_messenger_ != VK_NULL_HANDLE) {
                vkDestroyDebugUtilsMessengerEXT(instance_, debug_messenger_, nullptr);
            }

            vkDestroyInstance(instance_, nullptr);
        }
    }

    void Graphics::initializeVulkan()
    {
        createInstance();
        setupDebugMessenger();
        createSurface();
        pickPhysicalDevice();
        createLogicalDeviceAndQueues();
    }
}
