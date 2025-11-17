#pragma once

namespace vulkanEng
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
