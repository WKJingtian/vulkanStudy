#pragma once
#include "vulkanRendersystem.hpp"
#include "vulkanLightRendersystem.hpp"
#include "keyboardInput.hpp"
#include "vulkanDescriptor.hpp"

class vulkanApp
{
	public:
	static constexpr int WIDTH = 800;
	static constexpr int HEIGHT = 600;
	vulkanApp();
	~vulkanApp();
	vulkanApp(const vulkanApp &) = delete;
	vulkanApp &operator=(const vulkanApp &) = delete;
	void run();
	void loadGameObjects();
	vulkanWindow window{WIDTH, HEIGHT, "Vulkan Study"};
	vulkanDevice device{window};
	vulkanRenderer renderer{window, device};
	std::unique_ptr<vulkanDescriptorPool> globalDescriptorPool{};
	std::map<uint32_t, vulkanGameobject> gameObjects;
};