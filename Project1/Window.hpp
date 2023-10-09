#pragma once
#include "dependencies.hpp"
#include "Instance.hpp"

class Window {

public:
	Window(uint32_t width, uint32_t height, const std::string& title);
	~Window();

	void initialize();
	void createWindow();
	void createSurface();
	void createSwapchain();
	std::vector<VkImage>& getSwapchainImages();
	
	GLFWwindow* getWindow() { return window; }
	uint32_t getWidth() { return width; }
	uint32_t getHeight() { return height; }
	VkSurfaceKHR getSurface() { return surface; }
	VkFormat getFormat() { return surfaceFormat.format; }
	VkExtent2D getExtent() { return currentExtent; }
	VkPresentModeKHR getPresentMode() { return presentMode; }
	VkSwapchainKHR getSwapchain() { return swapchain; }

	Instance* instance;
	uniformBufferObject MVP;
	std::vector<bool> indexAvailable;

private:
	GLFWwindow* window;
	VkSurfaceKHR surface;

	

	VkSwapchainKHR swapchain;
	std::vector<VkImage> swapchainImages;

	VkSurfaceFormatKHR surfaceFormat;
	VkPresentModeKHR presentMode;
	VkExtent2D currentExtent;


	uint32_t  width;
	uint32_t  height;
	std::string title;




};