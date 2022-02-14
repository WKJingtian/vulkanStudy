#pragma once
#include "vulkanGameobject.hpp"
#include "vulkanPipeline.hpp"
#include "vulkanCamera.hpp"
#include "vulkanRendersystem.hpp"

class vulkanLightRendersystem
{
public:
	vulkanLightRendersystem(vulkanDevice &dev, VkRenderPass pas, VkDescriptorSetLayout setLayout);
	~vulkanLightRendersystem();
	vulkanLightRendersystem(const vulkanLightRendersystem &) = delete;
	vulkanLightRendersystem &operator=(const vulkanLightRendersystem &) = delete;
	void renderGameObjects(FrameInfo& frame);
		//,std::vector<vulkanGameobject> &gameObjects);
	void createPipelineLayout(VkDescriptorSetLayout setLayout);
	void createPipeline(VkRenderPass renderPass);
	void update(FrameInfo& info, globalUBO& ubo);
	vulkanDevice &device;
	std::unique_ptr<vulkanPipeline> pipeline;
	VkPipelineLayout layout;
};