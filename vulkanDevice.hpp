#pragma once
#include "vulkanWindow.hpp"

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices
{
    uint32_t graphicsFamily;
    uint32_t presentFamily;
    bool graphicsFamilyHasValue = false;
    bool presentFamilyHasValue = false;
    bool isComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
};

class vulkanDevice {
    public:
#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif
    vulkanDevice(vulkanWindow &win);
    ~vulkanDevice();
    // Not copyable or movable
    vulkanDevice(const vulkanDevice&) = delete;
    vulkanDevice& operator=(const vulkanDevice&) = delete;
    vulkanDevice(vulkanDevice&&) = delete;
    vulkanDevice& operator=(vulkanDevice&&) = delete;
    
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    VkFormat findSupportedFormat(
        const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    // Buffer Helper Functions
    void createBuffer(
        VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
        VkBuffer &buffer, VkDeviceMemory &bufferMemory);
    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void copyBufferToImage(
        VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);
    void createImageWithInfo(
        const VkImageCreateInfo &imageInfo, VkMemoryPropertyFlags properties,
        VkImage &image, VkDeviceMemory &imageMemory);
    void createInstance();
    void setupDebugMessenger();
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createCommandPool();
    // helper functions
    bool isDeviceSuitable(VkPhysicalDevice dev);
    std::vector<const char *> getRequiredExtensions();
    bool checkValidationLayerSupport();
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice dev);
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
    void hasGflwRequiredInstanceExtensions();
    bool checkDeviceExtensionSupport(VkPhysicalDevice dev);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice dev);

    VkPhysicalDeviceProperties properties;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    vulkanWindow &window;
    VkCommandPool commandPool;
    VkDevice device;
    VkSurfaceKHR surface;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    
    const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
};