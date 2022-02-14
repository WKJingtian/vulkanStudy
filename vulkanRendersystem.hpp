#pragma once
#include "vulkanGameobject.hpp"
#include "vulkanPipeline.hpp"
#include "vulkanCamera.hpp"

#define MAX_LIGHT_NUM 10
struct pointLight
{
	glm::vec4 pos{};
	glm::vec4 col{};
};
//global uniform buffer object
struct globalUBO
{
	glm::mat4 projection{ 1 };
	glm::mat4 view{ 1 };
	glm::vec4 ambientLight{ 1.8f, 1.6f, 1.6f, 0.001f };
	glm::vec4 camPos{ 0, 0, 0, 0 };
	pointLight lights[MAX_LIGHT_NUM];
	int numLight;
};
struct FrameInfo
{
	int frameIndex;
	float frameTime;
	VkCommandBuffer commandBuffer;
	vulkanCamera& camera;
	VkDescriptorSet globalSet;
	std::map<unsigned int, vulkanGameobject>& sceneObjects;
};

class vulkanRendersystem
{
public:
	vulkanRendersystem(vulkanDevice &dev, VkRenderPass pas, VkDescriptorSetLayout setLayout);
	~vulkanRendersystem();
	vulkanRendersystem(const vulkanRendersystem &) = delete;
	vulkanRendersystem &operator=(const vulkanRendersystem &) = delete;
	void renderGameObjects(FrameInfo& frame);
		//,std::vector<vulkanGameobject> &gameObjects);
	void createPipelineLayout(VkDescriptorSetLayout setLayout);
	void createPipeline(VkRenderPass renderPass);
	vulkanDevice &device;
	std::unique_ptr<vulkanPipeline> pipeline;
	VkPipelineLayout layout;
};