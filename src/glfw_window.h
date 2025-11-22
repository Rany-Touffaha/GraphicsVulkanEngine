#pragma once

struct GLFWwindow;

namespace vulkanEng
{
   class Window
   {
    public:
        Window(gsl::czstring name, glm::ivec2 size);
        ~Window();

        glm::ivec2 getWindowSize() const;
        glm::ivec2 getFramebufferSize() const;
        bool shouldClose() const;
        GLFWwindow* getHandle() const;

        bool tryMoveToMonitor(std::uint16_t monitor_number);

   private:
        GLFWwindow* window_;

   };
}
