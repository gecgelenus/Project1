#include "Instance.hpp"

Instance::Instance(){
}

Instance::~Instance() {
}

void Instance::initialize() {
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;


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

void Instance::pickPhysicalDevice() {
	uint32_t deviceCount = 0;
    bool selected = false;

	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
	std::cout << "Device count: " << deviceCount << std::endl;
    if (deviceCount == 0) {
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}
    else
    {
		std::cout << "Found " << deviceCount << " GPUs with Vulkan support!" << std::endl;
	}
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    for (const auto& device : devices) {
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		std::cout << "Device name: " << deviceProperties.deviceName << std::endl;
        if (deviceProperties.deviceType == 0)
        {
			std::cout << "Device type: " << "VK_PHYSICAL_DEVICE_TYPE_OTHER" << std::endl;
		}
        else if (deviceProperties.deviceType == 1)
        {
			std::cout << "Device type: " << "VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU" << std::endl;
		}
        else if (deviceProperties.deviceType == 2)
        {
			std::cout << "Device type: " << "VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU" << std::endl;
            physicalDevice = device;
            std::cout << "Picked physical device: " << deviceProperties.deviceName << std::endl;
            selected = true;
        }
        else if (deviceProperties.deviceType == 3)
        {
			std::cout << "Device type: " << "VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU" << std::endl;
		}
        else if (deviceProperties.deviceType == 4)
        {
			std::cout << "Device type: " << "VK_PHYSICAL_DEVICE_TYPE_CPU" << std::endl;
		}
        else
        {
			std::cout << "Device type: " << "Unknown" << std::endl;
		}
	}

    if (!selected) {
        physicalDevice = devices[0];
        std::cout << "Picked first physical device" << std::endl;
    }
	
}

void Instance::createLogicalDevice() {

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



