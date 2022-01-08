#pragma once
#include "vulkanSwapchain.hpp"

class vulkanRenderer {
public:
	vulkanRenderer(vulkanWindow &win, vulkanDevice &dev);
	~vulkanRenderer();
	vulkanRenderer(const vulkanRenderer &) = delete;
	inline vulkanRenderer &operator=(const vulkanRenderer &) = delete;
	inline VkRenderPass getSwapChainRenderPass() const { return swapchain->renderPass; }
	inline float getAspectRatio() const { return swapchain->extentAspectRatio(); }
	inline bool isFrameInProgress() const { return isFrameStarted; }
	VkCommandBuffer getCurrentCommandBuffer() const
	{
		assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
		return commandBuffers[currentFrameIndex];
	}
	inline int getFrameIndex() const
	{
		assert(isFrameStarted && "Cannot get frame index when frame not in progress");
		return currentFrameIndex;
	}
	VkCommandBuffer beginFrame();
	void endFrame();
	void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
	void endSwapChainRenderPass(VkCommandBuffer commandBuffer);
	void createCommandBuffers();
	void freeCommandBuffers();
	void recreateSwapChain();
	vulkanWindow &window;
	vulkanDevice &device;
	std::unique_ptr<vulkanSwapchain> swapchain;
	std::vector<VkCommandBuffer> commandBuffers;
	uint32_t currentImageIndex;
	int currentFrameIndex{0};
	bool isFrameStarted{false};
};