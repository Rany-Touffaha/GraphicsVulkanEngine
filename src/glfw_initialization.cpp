#include <GLFW/glfw3.h>
#include <glfw_initialization.h>
#include <precomp.h>
#include <spdlog/spdlog.h>

namespace vulkanEng
{
    void glfw_error_callback(std::int32_t error_code, gsl::czstring message)
    {
        spdlog::error("GLFW Error ({}): {}", error_code, message);
    }

    GLFWInitialization::GLFWInitialization()
    {
        glfwSetErrorCallback(glfw_error_callback);

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
