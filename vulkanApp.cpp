#include "pch.h"
#include "vulkanApp.hpp"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

vulkanApp::vulkanApp()
{
    globalDescriptorPool =
        vulkanDescriptorPool::Builder(device)
        .setMaxSets(vulkanSwapchain::MAX_FRAMES_IN_FLIGHT)
        .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, vulkanSwapchain::MAX_FRAMES_IN_FLIGHT)
        .build();
    loadGameObjects();
}
vulkanApp::~vulkanApp() {}
void vulkanApp::run()
{
    std::vector<std::unique_ptr<vulkanBuffer>> uboBuffers(vulkanSwapchain::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < vulkanSwapchain::MAX_FRAMES_IN_FLIGHT; i++)
    {
        uboBuffers[i] = std::make_unique<vulkanBuffer>(
            device, sizeof(globalUBO), 1,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
            device.properties.limits.minUniformBufferOffsetAlignment);
        uboBuffers[i]->map();
    }

    auto globalSetLayout =
        vulkanDescriptorSetLayout::Builder(device)
        .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
        .build();
    std::vector<VkDescriptorSet> globalDescriptorSets(vulkanSwapchain::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < globalDescriptorSets.size(); i++)
    {
        auto bufferInfo = uboBuffers[i]->descriptorInfo();
        vulkanDescriptorWriter(*globalSetLayout, *globalDescriptorPool)
            .writeBuffer(0, &bufferInfo)
            .build(globalDescriptorSets[i]);
    }

    vulkanRendersystem simpleRenderSystem{
        device, renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
    vulkanLightRendersystem lightRenderSystem{
        device, renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout() };
    vulkanCamera camera{};
    vulkanGameobject camObj = vulkanGameobject::createGameObject();
    keyboardInput keyboardController{};
    float x = 0;

    auto curTime = std::chrono::high_resolution_clock::now();
    while (!glfwWindowShouldClose(window.window))
    {
        glfwPollEvents();
        auto newTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - curTime).count();
        curTime = newTime;
        keyboardController.moveInPlaneXZ(window.window, deltaTime, camObj);
        float aspectRatio = renderer.getAspectRatio();
        //camera.setOrthographicStats(-aspectRatio, aspectRatio, -1, 1, -10, 10);
        camera.setPerspectiveStats(glm::radians(60.0f), aspectRatio, 0.1f, 100);
        camera.setViewXYZ(camObj.transform.translation, camObj.transform.rotation);
        if (auto commandBuffer = renderer.beginFrame())
        {
            int frameIndex = renderer.getFrameIndex();
            FrameInfo frameInfo{ frameIndex,
                deltaTime, commandBuffer,
                camera, globalDescriptorSets[frameIndex],
                gameObjects};
            globalUBO temp{};
            temp.projection = camera.projectionMatrix;
            temp.view = camera.viewMatrix;
            temp.camPos = glm::vec4(camera.camPos, 1);
            lightRenderSystem.update(frameInfo, temp);
            uboBuffers[frameIndex]->writeToBuffer(&temp);
            uboBuffers[frameIndex]->flush();

            renderer.beginSwapChainRenderPass(commandBuffer);
            simpleRenderSystem.renderGameObjects(frameInfo);
            lightRenderSystem.renderGameObjects(frameInfo);
            renderer.endSwapChainRenderPass(commandBuffer);
            renderer.endFrame();
        }
    }
    vkDeviceWaitIdle(device.device);
}

void vulkanApp::loadGameObjects()
{
    std::shared_ptr<vulkanModel> lveModel = vulkanModel::createModel(device, "models/smooth_vase.obj");
    auto cube1 = vulkanGameobject::createGameObject();
    cube1.model = lveModel;
    cube1.transform.translation = {.0f, .0f, 0};
    cube1.transform.scale = {3, 3, 3};
    gameObjects.emplace(cube1.getId(), std::move(cube1));
    auto cube2 = vulkanGameobject::createGameObject();
    cube2.model = lveModel;
    cube2.transform.translation = { 3.0f, .0f, 0 };
    cube2.transform.scale = { 3, 3, 3 };
    gameObjects.emplace(cube2.getId(), std::move(cube2));
    std::shared_ptr<vulkanModel> floorModel =
        vulkanModel::createModel(device, "models/floor.obj");
    auto floor = vulkanGameobject::createGameObject();
    floor.model = floorModel;
    floor.transform.translation = { 2.0f, .0f, 0 };
    floor.transform.scale = { 15, 1, 15 };
    gameObjects.emplace(floor.getId(), std::move(floor));

    for (int i = 0; i < 10; i++)
    {
        auto pointLight = vulkanGameobject::makePointLight(
            glm::vec4(static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
                static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
                static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
                10.0f), 0.01f
        );
        pointLight.transform.translation = { 0.05f * (i - 5), 1.0f, 0.0f };
        gameObjects.emplace(pointLight.getId(), std::move(pointLight));
    }
}