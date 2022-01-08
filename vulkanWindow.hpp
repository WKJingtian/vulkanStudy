#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class vulkanWindow
{
public:
	vulkanWindow(int w, int h, std::string n);
	~vulkanWindow();
	vulkanWindow(const vulkanWindow&) = delete;
	vulkanWindow& operator=(const vulkanWindow&) = delete;
	VkExtent2D getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }

	void createWindowSurface(VkInstance i, VkSurfaceKHR *s);
	static void framebufferResizeCallback(GLFWwindow *win, int w, int h);
	void init();

	int width;
	int height;
	bool framebufferResized = false;
	std::string windowName;
	GLFWwindow *window;
};
