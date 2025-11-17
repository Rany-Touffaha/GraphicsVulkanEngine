#include <GLFW/glfw3.h>
#include <glfw_initialization.h>

std::int32_t main(std::int32_t argc, gsl::zstring* argv)
{
    const vulkanEng::GLFWInitialization _glfw;
    
    gsl::not_null window = glfwCreateWindow(800, 600, "Vulkan Engine", nullptr, nullptr);
    gsl::final_action cleanup_window([window]()
    {
        glfwDestroyWindow(window);
    });
    
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }
    
    return EXIT_SUCCESS;
}