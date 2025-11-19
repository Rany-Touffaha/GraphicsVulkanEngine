#include <GLFW/glfw3.h>
#include <glfw_initialization.h>
#include <glfw_monitor.h>

std::int32_t main(std::int32_t argc, gsl::zstring* argv)
{
    const vulkanEng::GLFWInitialization _glfw;
    
    gsl::not_null window = glfwCreateWindow(800, 600, "Vulkan Engine", nullptr, nullptr);
    gsl::final_action cleanup_window([window]()
    {
        glfwDestroyWindow(window);
    });

    glm::ivec2 window_size;
    glfwGetWindowSize(window, &window_size.x, &window_size.y);

    gsl::span<GLFWmonitor*> monitors = vulkanEng::getMonitors();

    if (monitors.size() > 1) {
        vulkanEng::moveWindowToMonitor(window, monitors[1]);
    }

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }
    
    return EXIT_SUCCESS;
}