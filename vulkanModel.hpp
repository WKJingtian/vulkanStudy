#pragma once
#include "vulkanDevice.hpp"
#include "vulkanBuffer.hpp"
#include "glm/glm.hpp"

// from: https://stackoverflow.com/a/57595105
template <typename T, typename... Rest>
void hashCombine(std::size_t& seed, const T& v, const Rest&... rest)
{
	seed ^= std::hash<T>{}(v)+0x9e3779b9 + (seed << 6) + (seed >> 2);
	(hashCombine(seed, rest), ...);
};

class vulkanModel
{
public:
	struct Vertex
	{
		glm::vec3 position{};
		glm::vec3 color{};
		glm::vec3 normal{};
		glm::vec2 uv{};

		static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
		static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

		bool operator==(const Vertex& other) const
		{
			return position == other.position
				&& color == other.color
				&& normal == other.normal
				&& uv == other.uv;
		}
	};
	struct builder
	{
		std::vector<Vertex> vertices{};
		std::vector<uint32_t> indices{};

		void loadModel(const std::string& path);
	};
	vulkanModel(vulkanDevice& dev, const builder& model);
	~vulkanModel();
	vulkanModel(const vulkanModel &) = delete;
	vulkanModel &operator=(const vulkanModel &) = delete;
	void bind(VkCommandBuffer commandBuffer);
	void draw(VkCommandBuffer commandBuffer);
	void createVertexBuffers(const std::vector<Vertex>& vertices);
	void createIndexBuffers(const std::vector<uint32_t>& indices);
	static std::unique_ptr<vulkanModel> createModel(vulkanDevice& dev, const std::string& path);
	vulkanDevice &device;
	std::unique_ptr<vulkanBuffer> vertexBuffer;
	uint32_t vertexCount;
	std::unique_ptr<vulkanBuffer> indexBuffer;
	uint32_t indexCount;
};
