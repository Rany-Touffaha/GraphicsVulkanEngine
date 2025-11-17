#include <glfw_initialization.h>
#include <cstdlib>
#include <GLFW/glfw3.h>

namespace VulkanEng
{
    GLFWInitialization::GLFWInitialization()
    {
        if (glfwInit() != GLFW_TRUE)
        {
            std::exit(EXIT_FAILURE);
        }
    }
    
    GLFWInitialization::~GLFWInitialization()
    {
        glfwTerminate();
    }
    
}
