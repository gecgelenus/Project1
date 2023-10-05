#pragma once
#include "dependencies.hpp"

class Instance{

public:
	Instance();
	~Instance();

	void initialize();
	void pickPhysicalDevice();
	void createLogicalDevice();

	VkInstance getInstance() { return instance; }
	VkPhysicalDevice getPhysicalDevice() { return physicalDevice; }
	VkDevice getDevice() { return device; }
	VkQueue getGraphicsQueue() { return graphicsQueue; }

	VkCommandPool commandPool;

private:
	const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
	};

	const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	VkDevice device;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkInstance instance;


	VkQueue graphicsQueue;
};