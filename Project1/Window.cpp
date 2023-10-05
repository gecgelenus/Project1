#include "Window.hpp"

Window::Window(uint32_t width, uint32_t height, const std::string& title){

	this->width = width;
	this->height = height;
	this->title = title;
}

Window::~Window()
{

}

void Window::initialize()
{
	glfwInit();
    MVP[0].model = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.0f, 1.0f));
    MVP[0].view = glm::lookAt(glm::vec3(18.0f, 18.0f, 18.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    MVP[0].proj = glm::perspective(glm::radians(45.0f), ((float)800 / 600), 0.1f, 100.0f);

    MVP[0].proj[1][1] *= -1;

    MVP[1].model = glm::translate(glm::mat4(1.0f), glm::vec3(9.0f, 5.0f, 5.0f));
    MVP[1].view = glm::lookAt(glm::vec3(18.0f, 18.0f, 18.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    MVP[1].proj = glm::perspective(glm::radians(45.0f), ((float)800 / 600), 0.1f, 100.0f);

    MVP[1].proj[1][1] *= -1;
}

void Window::createWindow()
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

}

void Window::createSurface()
{
	if (glfwCreateWindowSurface(instance->getInstance(), window, nullptr, &surface) != VK_FALSE) {
		throw std::runtime_error("Couldn't created window surface!");
	}
	else {
		std::cout << "Created window surface" << std::endl;
	}
}

void Window::createSwapchain() {

    

    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;

    uint32_t formatCount = 0;
    uint32_t presentModeCount = 0;


    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(instance->getPhysicalDevice(), surface, &capabilities);

    vkGetPhysicalDeviceSurfaceFormatsKHR(instance->getPhysicalDevice(), surface, &formatCount, nullptr);
    formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(instance->getPhysicalDevice(), surface, &formatCount, formats.data());

    vkGetPhysicalDeviceSurfacePresentModesKHR(instance->getPhysicalDevice(), surface, &presentModeCount, nullptr);
    presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(instance->getPhysicalDevice(), surface, &presentModeCount, presentModes.data());


    for (const auto& f : formats) {
        if (f.format == VK_FORMAT_B8G8R8A8_SRGB && f.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
            surfaceFormat = f;
    }

    presentMode = VK_PRESENT_MODE_MAILBOX_KHR;

    int width, height;

    glfwGetFramebufferSize(window, &width, &height);

    currentExtent = {
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height)
    };

    VkSwapchainCreateInfoKHR swapchainCreateInfo{};
    swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCreateInfo.surface = surface;
    swapchainCreateInfo.imageArrayLayers = 1;
    swapchainCreateInfo.clipped = VK_TRUE;
    swapchainCreateInfo.imageFormat = surfaceFormat.format;
    swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
    swapchainCreateInfo.imageExtent = currentExtent;
    swapchainCreateInfo.minImageCount = 3;
    swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchainCreateInfo.preTransform = capabilities.currentTransform;
    swapchainCreateInfo.presentMode = presentMode;
    swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(instance->getDevice(), &swapchainCreateInfo, nullptr, &swapchain) != VK_SUCCESS) {
        std::cerr << "Swapchain couldn't created" << std::endl;
    }
    else {
        std::cout << "Swapchain is created." << std::endl;
    }

    uint32_t imageCount;

    vkGetSwapchainImagesKHR(instance->getDevice(), swapchain, &imageCount, nullptr);
    swapchainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(instance->getDevice(), swapchain, &imageCount, swapchainImages.data());
    std::cout << "Swapchain images retrieved: " << swapchainImages.size() << std::endl;

    


}

std::vector<VkImage>& Window::getSwapchainImages()
{
    return swapchainImages;
}
