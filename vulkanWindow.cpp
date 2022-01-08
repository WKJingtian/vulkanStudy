#include "pch.h"
#include "vulkanWindow.hpp"

vulkanWindow::vulkanWindow(int w, int h, std::string n)
    : width{w}, height{h}, windowName{n}
    { init(); }

vulkanWindow::~vulkanWindow()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void vulkanWindow::init()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void vulkanWindow::createWindowSurface(VkInstance i, VkSurfaceKHR *s)
{
    if (glfwCreateWindowSurface(i, window, nullptr, s) != VK_SUCCESS)
        throw std::runtime_error("failed to craete window surface");
}

// 这是受GLFW规定的，在窗口大小变化时出发的callback
void vulkanWindow::framebufferResizeCallback(GLFWwindow *win, int w, int h)
{
  auto lveWindow = reinterpret_cast<vulkanWindow *>(glfwGetWindowUserPointer(win));
  lveWindow->framebufferResized = true;
  lveWindow->width = w;
  lveWindow->height = h;
}
