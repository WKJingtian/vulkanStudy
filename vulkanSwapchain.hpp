#pragma once
#include "vulkanDevice.hpp"
#include <vulkan/vulkan.h>

class vulkanSwapchain {
public:
    static constexpr int MAX_FRAMES_IN_FLIGHT = 2;
    
    vulkanSwapchain(vulkanDevice& dev, VkExtent2D windowExtent);
    vulkanSwapchain(vulkanDevice& dev, VkExtent2D windowExtent, std::shared_ptr<vulkanSwapchain> previous);
    ~vulkanSwapchain();
    vulkanSwapchain(const vulkanSwapchain &) = delete;
    vulkanSwapchain &operator=(const vulkanSwapchain &) = delete;
    inline VkFramebuffer getFrameBuffer(int index) { return swapChainFramebuffers[index]; }
    inline VkImageView getImageView(int index) { return swapChainImageViews[index]; }
    inline size_t imageCount() { return swapChainImages.size(); }
    inline uint32_t width() { return swapChainExtent.width; }
    inline uint32_t height() { return swapChainExtent.height; }
    inline float extentAspectRatio()
    { return static_cast<float>(swapChainExtent.width) / static_cast<float>(swapChainExtent.height); }
    VkFormat findDepthFormat();
    VkResult acquireNextImage(uint32_t *imageIndex);
    VkResult submitCommandBuffers(const VkCommandBuffer *buffers, uint32_t *imageIndex);
    bool compareSwapFormats(const vulkanSwapchain& swapChain) const;
    void init();
    void createSwapChain();
    void createImageViews();
    void createDepthResources();
    void createRenderPass();
    void createFramebuffers();
    void createSyncObjects();
    // Helper functions
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
    VkFormat swapChainImageFormat;
    VkFormat swapChainDepthFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    VkRenderPass renderPass;
    std::vector<VkImage> depthImages;
    std::vector<VkDeviceMemory> depthImageMemorys;
    std::vector<VkImageView> depthImageViews;
    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;
    vulkanDevice &device;
    VkExtent2D windowExtent;
    VkSwapchainKHR swapChain;
    std::shared_ptr<vulkanSwapchain> oldSwapChain;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;
    size_t currentFrame = 0;
};
