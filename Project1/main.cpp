#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <vector>

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

GLFWwindow* window;

VkInstance instance;
VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
VkDevice device;
VkQueue graphicsQueue;

VkSurfaceKHR surface;
VkSwapchainKHR swapchain;
VkExtent2D currentExtent;
VkSurfaceFormatKHR surfaceFormat;
VkPresentModeKHR presentMode;
std::vector<VkImage> swapchainImages;
std::vector<VkImageView> swapchainImageViews;

void initVulkan();
void createSurface();
void pickPhysicalDevice();
void createLogicalDevice();
void createSwapchain();
void createImageViews();


int main() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

    initVulkan();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapchain();
    createImageViews();


    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}


void initVulkan() {
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;


    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);


    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = validationLayers.size();
    createInfo.ppEnabledLayerNames = validationLayers.data();

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
    else
    {
        std::cout << "Vulkan instance created." << std::endl;
    }
}

void createSurface() {

    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_FALSE) {
        throw std::runtime_error("Couldn't created window surface!");
    }
    else {
        std::cout << "Created window surface" << std::endl;
    }

}

void pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices.data());

    if (physicalDevices.size() <= 0) {
        std::cout << "No physical device found!" << std::endl;
        return;
    }
    else {
        std::cout << "Physical device count: " << deviceCount << std::endl;
    }


    for (VkPhysicalDevice pDevice : physicalDevices) {
        VkPhysicalDeviceProperties properties = {};
        vkGetPhysicalDeviceProperties(pDevice, &properties);
        std::cout << "--------------------------------" << std::endl;
        std::cout << "Device Name: " << properties.deviceName << std::endl;
        std::cout << "Device Type:" << properties.deviceType << std::endl;
        std::cout << "--------------------------------" << std::endl;
        if (properties.deviceType == 2) {
            physicalDevice = pDevice;
        }
    
    }



}

void createLogicalDevice() {
    
    const float priorities = 1.0f;

    VkDeviceQueueCreateInfo queueInfo = {};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueFamilyIndex = 0;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = &priorities;


    VkPhysicalDeviceFeatures physicalDeviceFeatures{};

    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = &queueInfo;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;
    deviceCreateInfo.enabledExtensionCount = deviceExtensions.size();
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
    deviceCreateInfo.enabledLayerCount = 1;
    deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();


    if (vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device) != VK_FALSE) {
        std::cout << "Device couldn't created" << std::endl;
    }

    vkGetDeviceQueue(device, 0, 0, &graphicsQueue);
}

void createSwapchain() {

    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;

    uint32_t formatCount = 0;
    uint32_t presentModeCount = 0;


    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities);

    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
    formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats.data());

    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
    presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());

  
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

    if (vkCreateSwapchainKHR(device, &swapchainCreateInfo, nullptr, &swapchain) != VK_SUCCESS) {
        std::cerr << "Swapchain couldn't created" << std::endl;
    }
    else {
        std::cout << "Swapchain is created." << std::endl;
    }

    uint32_t imageCount;

    vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr);
    swapchainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device, swapchain, &imageCount, swapchainImages.data());
    std::cout << "Swapchain images retrieved: " << swapchainImages.size() << std::endl;




}

void createImageViews() {

    swapchainImageViews.resize(swapchainImages.size());

    bool flag = true;

    for (int i = 0; i < swapchainImageViews.size(); i++) {
        VkImageViewCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        info.image = swapchainImages[i];
        info.format = surfaceFormat.format;
        info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        info.subresourceRange.baseArrayLayer = 0;
        info.subresourceRange.layerCount = 1;
        info.subresourceRange.baseMipLevel = 0;
        info.subresourceRange.levelCount = 1;
        info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;

        if (vkCreateImageView(device, &info, nullptr, &swapchainImageViews[i]) != VK_SUCCESS) {
            std::cerr << "Image view couldn't created." << std::endl;
            flag = false;
            break;
        }

    }

    if (flag) {
        std::cout << "Image views are created." << std::endl;
    }


}
