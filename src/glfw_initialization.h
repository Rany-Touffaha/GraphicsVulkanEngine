#pragma once

namespace VulkanEng
{
    struct GLFWInitialization
    {
        public:
            GLFWInitialization();
            ~GLFWInitialization();
        
            GLFWInitialization(const GLFWInitialization&) = delete;
            GLFWInitialization& operator=(const GLFWInitialization&) = delete;
    };
    
}
