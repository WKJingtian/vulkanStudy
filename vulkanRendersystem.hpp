#pragma once
#include "vulkanGameobject.hpp"
#include "vulkanPipeline.hpp"
#include "vulkanCamera.hpp"

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