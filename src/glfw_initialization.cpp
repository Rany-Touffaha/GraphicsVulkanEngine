#include <GLFW/glfw3.h>
#include <glfw_initialization.h>

namespace vulkanEng
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
