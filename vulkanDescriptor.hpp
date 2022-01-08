#pragma once
#include "vulkanDevice.hpp"

class vulkanDescriptorSetLayout
{
public:
    class Builder
    {
    public:
        Builder(vulkanDevice& dev) : device{ dev } {}
        Builder& addBinding(
            uint32_t binding,
            VkDescriptorType descriptorType,
            VkShaderStageFlags stageFlags,
            uint32_t count = 1);
        std::unique_ptr<vulkanDescriptorSetLayout> build() const;
        vulkanDevice& device;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
    };

    vulkanDescriptorSetLayout(
        vulkanDevice& dev, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
    ~vulkanDescriptorSetLayout();
    vulkanDescriptorSetLayout(const vulkanDescriptorSetLayout&) = delete;
    vulkanDescriptorSetLayout& operator=(const vulkanDescriptorSetLayout&) = delete;
    VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }
    vulkanDevice& device;
    VkDescriptorSetLayout descriptorSetLayout;
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;
};

class vulkanDescriptorPool
{
public:
    class Builder
    {
    public:
        Builder(vulkanDevice& dev) : device{ dev } {}
        Builder& addPoolSize(VkDescriptorType descriptorType, uint32_t count);
        Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags);
        Builder& setMaxSets(uint32_t count);
        std::unique_ptr<vulkanDescriptorPool> build() const;
        vulkanDevice& device;
        std::vector<VkDescriptorPoolSize> poolSizes{};
        uint32_t maxSets = 1000;
        VkDescriptorPoolCreateFlags poolFlags = 0;
    };

    vulkanDescriptorPool(
        vulkanDevice& dev,
        uint32_t maxSets,
        VkDescriptorPoolCreateFlags poolFlags,
        const std::vector<VkDescriptorPoolSize>& poolSizes);
    ~vulkanDescriptorPool();
    vulkanDescriptorPool(const vulkanDescriptorPool&) = delete;
    vulkanDescriptorPool& operator=(const vulkanDescriptorPool&) = delete;
    bool allocateDescriptor(
        const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;
    void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;
    void resetPool();

    vulkanDevice& device;
    VkDescriptorPool descriptorPool;
};

class vulkanDescriptorWriter
{
public:
    vulkanDescriptorWriter(vulkanDescriptorSetLayout& setLayout, vulkanDescriptorPool& pool);
    vulkanDescriptorWriter& writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
    vulkanDescriptorWriter& writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);
    bool build(VkDescriptorSet& set);
    void overwrite(VkDescriptorSet& set);
    vulkanDescriptorSetLayout& setLayout;
    vulkanDescriptorPool& pool;
    std::vector<VkWriteDescriptorSet> writes;
};