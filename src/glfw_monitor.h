#pragma once

struct GLFWmonitor;
struct GLFWwindow;

namespace vulkanEng
{
    gsl::span<GLFWmonitor*> getMonitors();
    glm::ivec2 getMonitorPosition(gsl::not_null<GLFWmonitor*> monitor);
    glm::ivec2 getMonitorWorkarea(gsl::not_null<GLFWmonitor*> monitor);
    void moveWindowToMonitor(gsl::not_null<GLFWwindow*> window, gsl::not_null<GLFWmonitor*> monitor);
}
