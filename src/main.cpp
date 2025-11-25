#include <precomp.h>
#include <GLFW/glfw3.h>
#include <glfw_initialization.h>
#include <glfw_monitor.h>
#include <glfw_window.h>
#include <graphics.h>

std::int32_t main(std::int32_t argc, gsl::zstring* argv)
{
    const vulkanEng::GLFWInitialization _glfw;

    vulkanEng::Window window("Vulkan Engine", {800, 600});
    window.tryMoveToMonitor(1);

    vulkanEng::Graphics graphics(&window);

    std::array<vulkanEng::Vertex, 3> vertices = {
        vulkanEng::Vertex{glm::vec3{0.0f, -0.5f, 0.0f}, glm::vec3{1.0f, 0.0f, 0.0f}},
        vulkanEng::Vertex{glm::vec3{0.5f, 0.5f, 0.0f}, glm::vec3{0.0f, 1.0f, 0.0f}},
        vulkanEng::Vertex{glm::vec3{-0.5f, 0.5f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f}}
    };

    vulkanEng::BufferHandle buffer = graphics.createVertexBuffer(vertices);

    while (!window.shouldClose())
    {
        glfwPollEvents();
        if(graphics.BeginFrame()) {
            graphics.RenderBuffer(buffer, vertices.size());
            graphics.EndFrame();
        }
    }

    graphics.destroyBuffer(buffer);
    
    return EXIT_SUCCESS;
}