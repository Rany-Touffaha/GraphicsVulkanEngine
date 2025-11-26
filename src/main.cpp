#include <precomp.h>
#include <GLFW/glfw3.h>
#include <glfw_initialization.h>
#include <glfw_monitor.h>
#include <glfw_window.h>
#include <graphics.h>
#include <glm/gtc/matrix_transform.hpp>

std::int32_t main(std::int32_t argc, gsl::zstring* argv)
{
    const vulkanEng::GLFWInitialization _glfw;

    vulkanEng::Window window("Vulkan Engine", {800, 600});
    window.tryMoveToMonitor(1);

    vulkanEng::Graphics graphics(&window);

    std::array<vulkanEng::Vertex, 4> vertices = {
        vulkanEng::Vertex{glm::vec3{-0.5f, -0.5f, 0.0f}, glm::vec2{0.0f, 1.0f}},
        vulkanEng::Vertex{glm::vec3{0.5f, -0.5f, 0.0f}, glm::vec2{1.0f, 1.0f}},
        vulkanEng::Vertex{glm::vec3{-0.5f, 0.5f, 0.0f}, glm::vec2{0.0f, 0.0f}},
        vulkanEng::Vertex{glm::vec3{0.5f, 0.5f, 0.0f}, glm::vec2{1.0f, 0.0f}},
    };

    vulkanEng::BufferHandle buffer = graphics.createVertexBuffer(vertices);

    std::array<std::uint32_t, 6> indices = {0, 3, 2, 0, 1, 3};

    vulkanEng::BufferHandle index_buffer = graphics.createIndexBuffer(indices);

    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    
    graphics.SetViewProjection(view, projection);
    vulkanEng::TextureHandle texture = graphics.createTexture("VSCodeProjects/GraphicsVulkanEngine/paving-stones.jpg");

    while (!window.shouldClose())
    {
        glfwPollEvents();
        if(graphics.BeginFrame()){
            graphics.SetTexture(texture);
            graphics.RenderIndexedBuffer(index_buffer, buffer, indices.size());
            graphics.EndFrame();
        }
    }

    graphics.destroyTexture(texture);
    graphics.destroyBuffer(buffer);
    graphics.destroyBuffer(index_buffer);
    
    return EXIT_SUCCESS;
}