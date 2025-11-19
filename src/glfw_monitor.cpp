#include <glfw_monitor.h>
#include <GLFW/glfw3.h>

namespace vulkanEng
{
    gsl::span<GLFWmonitor*> getMonitors() {
        std::int32_t monitor_count = 0;
        GLFWmonitor** monitor_pointers = glfwGetMonitors(&monitor_count);
        return gsl::span<GLFWmonitor*>(monitor_pointers, monitor_count);
    }

    glm::ivec2 getMonitorPosition(gsl::not_null<GLFWmonitor*> monitor) {
        glm::ivec2 monitor_position;
        glfwGetMonitorPos(monitor, &monitor_position.x, &monitor_position.y);
        return monitor_position;
    }

    glm::ivec2 getMonitorWorkAreaSize(gsl::not_null<GLFWmonitor*> monitor) {
        glm::ivec2 monitor_size;
        glfwGetMonitorWorkarea(monitor, nullptr, nullptr, &monitor_size.x, &monitor_size.y);
        return monitor_size;
    }

    void moveWindowToMonitor(gsl::not_null<GLFWwindow*> window, gsl::not_null<GLFWmonitor*> monitor) {
        glm::ivec2 window_size;
        glfwGetWindowSize(window, &window_size.x, &window_size.y);

        const glm::ivec2 new_window_position = getMonitorPosition(monitor) + (getMonitorWorkAreaSize(monitor) / 2) - (window_size) / 2;
        glfwSetWindowPos(window, new_window_position.x, new_window_position.y);
    };
}
