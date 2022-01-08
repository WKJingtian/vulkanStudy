#pragma once
#include "vulkanRendersystem.hpp"
#include "vulkanLightRendersystem.hpp"
#include "keyboardInput.hpp"
#include "vulkanDescriptor.hpp"

//global uniform buffer object
struct globalUBO
{
	glm::mat4 projection{ 1 };
	glm::mat4 view{ 1 };
	//glm::vec3 lightDirection = glm::normalize(glm::vec3(10.0f, -1.0f, -1.0f));
	glm::vec4 ambientLight{ 1.8f, 1.6f, 1.6f, 0.001f };
	glm::vec4 lightPos{ 0, 1, 0, 0.0f };
	glm::vec4 lightColor{ 1, 1, 1, 5 };
};

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