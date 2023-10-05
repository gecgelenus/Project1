#pragma once
#include "dependencies.hpp"
#include "Instance.hpp"
#include "Window.hpp"



class Renderer{

public:
	Renderer(Window* window, Instance* instance);
	~Renderer();

	
	void createImageViews();
	void createRenderPass();
	void createGraphicsPipeline();
	void createFramebuffers();
	void createCommandPool();
	void createCommandBuffers();
	void createSyncObjects();
	void drawFrame();

private:
	Window* window;
	Instance* instance;
	VkRenderPass renderPass;

	VkDescriptorSetLayout descriptorLayout;
	VkPipeline graphicsPipeline;
	VkPipelineLayout pipelineLayout;
	


	std::vector<VkImage> swapchainImages;
	std::vector<VkImageView> swapchainImageViews;
	std::vector<VkFramebuffer> swapchainBuffers;

};