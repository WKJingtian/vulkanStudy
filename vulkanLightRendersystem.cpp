#include "pch.h"
#include "vulkanLightRendersystem.hpp"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

vulkanLightRendersystem::vulkanLightRendersystem(
    vulkanDevice& dev, VkRenderPass pas, VkDescriptorSetLayout setLayout)
    : device{dev}
{
    createPipelineLayout(setLayout);
    createPipeline(pas);
}
vulkanLightRendersystem::~vulkanLightRendersystem()
{
    vkDestroyPipelineLayout(device.device, layout, nullptr);
}
void vulkanLightRendersystem::createPipelineLayout(VkDescriptorSetLayout setLayout)
{
    // VkPushConstantRange pushConstantRange{};
    // pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    // pushConstantRange.offset = 0;
    // pushConstantRange.size = sizeof(SimplePushConstantData);
    std::vector<VkDescriptorSetLayout> descriptorSetLayouts{ setLayout };
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
    pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = 0;
    if (vkCreatePipelineLayout(device.device, &pipelineLayoutInfo, nullptr, &layout) !=
        VK_SUCCESS) throw std::runtime_error("failed to create pipeline layout!");
}

void vulkanLightRendersystem::createPipeline(VkRenderPass renderPass)
{
    assert(layout != nullptr && "Cannot create pipeline before pipeline layout");
    PipelineConfigInfo pipelineConfig{};
    vulkanPipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.attributeDescription.clear();
    pipelineConfig.bindingDescription.clear();
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = layout;
    pipeline = std::make_unique<vulkanPipeline>(
        "shaders/pointLight.vert.spv", "shaders/pointLight.frag.spv",
        device, pipelineConfig);
}

void vulkanLightRendersystem::renderGameObjects(FrameInfo& frame)
    //,std::vector<vulkanGameobject>& gameObjects)
{
    pipeline->bind(frame.commandBuffer);
    vkCmdBindDescriptorSets(frame.commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1,
        &frame.globalSet, 0, 0);
    vkCmdDraw(frame.commandBuffer, 6, 1, 0, 0);
}