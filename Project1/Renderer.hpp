#pragma once
#include "dependencies.hpp"
#include "Instance.hpp"
#include "Window.hpp"
#include "Object.hpp"


class Renderer{

public:
	Renderer(Window* window, Instance* instance);
	~Renderer();

	void initialize();

	void createImageViews();
	void createRenderPass();
	void createGraphicsPipeline();
	void createFramebuffers();
	void createCommandPool();
	void createCommandBuffers();
	void createTextureImage();

	void createDescriptorPool();
	void createDescriptorSetLayout();
	void allocateDescriptorSet();
	
	
	void createUniformBuffers();
	void createStagingBuffers();

	void updateVertexBuffer();
	void updateIndexBuffer();

	void recordCommandBuffer(VkCommandBuffer cbuffer, uint32_t imageIndex);
	void createSyncObjects();
	void drawFrame();

	void addObject(Object* object);
	void removeObject(Object* object);

	

	std::vector<VkCommandBuffer> commandBuffers;


private:
	Window* window;
	Instance* instance;
	VkRenderPass renderPass;

	VkDescriptorPool descriptorPool;
	VkDescriptorSetLayout descriptorLayout;
	std::vector<VkDescriptorSet> descriptorSets;

	VkSampler textureSampler;

	VkPipeline graphicsPipeline;
	VkPipelineLayout pipelineLayout;

	VkCommandPool commandPool;
	
	
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBufferMemories;
	std::vector<void*> uniformBufferMemoryMaps;

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	std::vector<VkSemaphore> renderSemaphores;
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkFence> frameFences;

	void createVertexBuffer();
	void createIndexBuffer();
	void updateUniformBuffers(uint32_t index);

	void createSampler();


	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
		VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

	void createTextureImageView();
	void createImage(uint32_t width, uint32_t height, VkFormat format,
		VkImageTiling tiling, VkImageUsageFlags usage,
		VkMemoryPropertyFlags properties, VkImage& image,
		VkDeviceMemory& imageMemory);

	void copyBuffers(VkBuffer stagingBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	
	void copyBufferToImage(VkBuffer stagingBuffer, VkImage dstImage, uint32_t width, uint32_t height);
	void imageTransitionLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);


	VkShaderModule createShaderModule(const std::vector<char>& code);

	std::vector<char> readFile(const std::string& filename);

	std::vector<VkImage> swapchainImages;
	std::vector<VkImageView> swapchainImageViews;
	std::vector<VkFramebuffer> swapchainBuffers;

	uint32_t currentFrame;

	std::vector<Object*> objects;
	std::vector<Vertex> vertices;
	std::vector<uint16_t> indicies;

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	VkBuffer stagingBufferVertex;
	VkDeviceMemory stagingBufferMemoryVertex;
	VkBuffer stagingBufferIndex;
	VkDeviceMemory stagingBufferMemoryIndex;


	VkImage textureImage;
	VkImageView textureImageView;
	VkDeviceMemory textureImageMemory;

};