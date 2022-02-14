#include "pch.h"
#include "vulkanLightRendersystem.hpp"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

struct pointLightPushConstant
{
    glm::vec4 pos{};
    glm::vec4 col{};
    float rad;
};

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
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(pointLightPushConstant);
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
    for (auto& item : frame.sceneObjects)
    {
        auto& obj = item.second;
        if (obj.pointLight)
        {
            pointLightPushConstant push{};
            push.pos = glm::vec4(obj.transform.translation, 1);
            push.col = obj.pointLight->lightColor;
            push.rad = obj.transform.scale.x;
            vkCmdPushConstants(
                frame.commandBuffer, layout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
                sizeof(pointLightPushConstant),
                &push
            );
            vkCmdDraw(frame.commandBuffer, 6, 1, 0, 0);
        }
    }
}

void vulkanLightRendersystem::update(FrameInfo& info, globalUBO& ubo)
{
    int lightIdx = 0;
    for (auto& item : info.sceneObjects)
    {
        auto& obj = item.second;
        if (obj.pointLight)
        {
            ubo.lights[lightIdx].pos = glm::vec4(obj.transform.translation, 1);
            ubo.lights[lightIdx].col = obj.pointLight->lightColor;
            lightIdx++;
        }
    }
    ubo.numLight = lightIdx;
}