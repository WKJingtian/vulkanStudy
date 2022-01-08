#pragma once
#include "vulkanModel.hpp"

struct PipelineConfigInfo
{
    PipelineConfigInfo() = default;
    PipelineConfigInfo(const PipelineConfigInfo&) = delete;
    PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;
    
    std::vector<VkVertexInputBindingDescription> bindingDescription{};
    std::vector<VkVertexInputAttributeDescription> attributeDescription{};
    VkPipelineViewportStateCreateInfo viewportInfo;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    std::vector<VkDynamicState> dynamicStateEnables;
    VkPipelineDynamicStateCreateInfo dynamicStateInfo;
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;
};

class vulkanPipeline
{
public:
    vulkanPipeline(
        const std::string& vertFilepath, const std::string& fragFilepath,
        vulkanDevice& dev, const PipelineConfigInfo& configInfo);
    ~vulkanPipeline();
    vulkanPipeline(const vulkanPipeline&) = delete;
    vulkanPipeline& operator=(const vulkanPipeline&) = delete;
    void bind(VkCommandBuffer commandBuffer);
    static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);
    static std::vector<char> readFile(const std::string& filepath);
    void createGraphicsPipeline(
        const std::string& vertFilepath, const std::string& fragFilepath,
        const PipelineConfigInfo& configInfo);
    void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
    vulkanDevice& device;
    VkPipeline graphicsPipeline;
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
};