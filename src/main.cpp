#include <GLFW/glfw3.h>
#include <glfw_initialization.h>
#include <glfw_monitor.h>
#include <glfw_window.h>
#include <precomp.h>

std::int32_t main(std::int32_t argc, gsl::zstring* argv)
{
    const vulkanEng::GLFWInitialization _glfw;

    vulkanEng::Window window("Vulkan Engine", {800, 600});
    window.tryMoveToMonitor(1);

    while (!window.shouldClose())
    {
        glfwPollEvents();
    }
    
    return EXIT_SUCCESS;
}