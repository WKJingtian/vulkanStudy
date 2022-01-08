#include "pch.h"
#include "vulkanRendersystem.hpp"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

struct SimplePushConstantData
{
    glm::mat4 modelMatrix{ 1.f };
    glm::mat4 normalMatrix{ 1.f };
};

vulkanRendersystem::vulkanRendersystem(
    vulkanDevice& dev, VkRenderPass pas, VkDescriptorSetLayout setLayout)
    : device{dev}
{
    createPipelineLayout(setLayout);
    createPipeline(pas);
}
vulkanRendersystem::~vulkanRendersystem()
{
    vkDestroyPipelineLayout(device.device, layout, nullptr);
}
void vulkanRendersystem::createPipelineLayout(VkDescriptorSetLayout setLayout)
{
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);
    std::vector<VkDescriptorSetLayout> descriptorSetLayouts{ setLayout };
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
    pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    if (vkCreatePipelineLayout(device.device, &pipelineLayoutInfo, nullptr, &layout) !=
        VK_SUCCESS) throw std::runtime_error("failed to create pipeline layout!");
}

void vulkanRendersystem::createPipeline(VkRenderPass renderPass)
{
    assert(layout != nullptr && "Cannot create pipeline before pipeline layout");
    PipelineConfigInfo pipelineConfig{};
    vulkanPipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = layout;
    pipeline = std::make_unique<vulkanPipeline>(
        "shaders/defaultShader.vert.spv", "shaders/defaultShader.frag.spv",
        device, pipelineConfig);
}

void vulkanRendersystem::renderGameObjects(FrameInfo& frame)
    //,std::vector<vulkanGameobject>& gameObjects)
{
    pipeline->bind(frame.commandBuffer);
    vkCmdBindDescriptorSets(frame.commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1,
        &frame.globalSet, 0, 0);
    for (auto& itemPair : frame.sceneObjects)
    {
        auto& obj = itemPair.second;
        SimplePushConstantData push{};
        push.modelMatrix = obj.transform.mat4();
        push.normalMatrix = obj.transform.normalMatrix();
        vkCmdPushConstants(frame.commandBuffer, layout,
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            0, sizeof(SimplePushConstantData), &push);
        obj.model->bind(frame.commandBuffer);
        obj.model->draw(frame.commandBuffer);
    }
}