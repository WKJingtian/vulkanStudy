#include "pch.h"
#include "vulkanBuffer.hpp"

VkDeviceSize vulkanBuffer::getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment)
{
    if (minOffsetAlignment > 0)
        return (instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
    return instanceSize;
}

vulkanBuffer::vulkanBuffer(vulkanDevice& device, VkDeviceSize instanceSize,
    uint32_t instanceCount, VkBufferUsageFlags usageFlags,
    VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment)
    : device{ device }, instanceSize{ instanceSize }, instanceCount{ instanceCount },
    usageFlags{ usageFlags }, memoryPropertyFlags{ memoryPropertyFlags }
{
    alignmentSize = getAlignment(instanceSize, minOffsetAlignment);
    bufferSize = alignmentSize * instanceCount;
    device.createBuffer(bufferSize, usageFlags, memoryPropertyFlags, buffer, memory);
}

vulkanBuffer::~vulkanBuffer()
{
    unmap();
    vkDestroyBuffer(device.device, buffer, nullptr);
    vkFreeMemory(device.device, memory, nullptr);
}

VkResult vulkanBuffer::map(VkDeviceSize size, VkDeviceSize offset)
{
    assert(buffer && memory && "Called map on buffer before create");
    return vkMapMemory(device.device, memory, offset, size, 0, &mapped);
}

void vulkanBuffer::unmap()
{
    if (mapped)
    {
        vkUnmapMemory(device.device, memory);
        mapped = nullptr;
    }
}

void vulkanBuffer::writeToBuffer(void* data, VkDeviceSize size, VkDeviceSize offset)
{
    assert(mapped && "Cannot copy to unmapped buffer");
    if (size == VK_WHOLE_SIZE)
        memcpy(mapped, data, bufferSize);
    else
    {
        char* memOffset = (char*)mapped;
        memOffset += offset;
        memcpy(memOffset, data, size);
    }
}

VkResult vulkanBuffer::flush(VkDeviceSize size, VkDeviceSize offset)
{
    VkMappedMemoryRange mappedRange = {};
    mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    mappedRange.memory = memory;
    mappedRange.offset = offset;
    mappedRange.size = size;
    return vkFlushMappedMemoryRanges(device.device, 1, &mappedRange);
}

VkResult vulkanBuffer::invalidate(VkDeviceSize size, VkDeviceSize offset)
{
    VkMappedMemoryRange mappedRange = {};
    mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    mappedRange.memory = memory;
    mappedRange.offset = offset;
    mappedRange.size = size;
    return vkInvalidateMappedMemoryRanges(device.device, 1, &mappedRange);
}

VkDescriptorBufferInfo vulkanBuffer::descriptorInfo(VkDeviceSize size, VkDeviceSize offset)
{
    return VkDescriptorBufferInfo
    {
        buffer,
        offset,
        size,
    };
}

void vulkanBuffer::writeToIndex(void* data, int index)
{
    writeToBuffer(data, instanceSize, index * alignmentSize);
}

VkResult vulkanBuffer::flushIndex(int index)
{
    return flush(alignmentSize, index * alignmentSize);
}

VkDescriptorBufferInfo vulkanBuffer::descriptorInfoForIndex(int index)
{
    return descriptorInfo(alignmentSize, index * alignmentSize);
}

VkResult vulkanBuffer::invalidateIndex(int index)
{
    return invalidate(alignmentSize, index * alignmentSize);
}