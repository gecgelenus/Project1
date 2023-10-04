#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#define MAX_FRAME_ON_PROCESS 3

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <vector>

struct uniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};



struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;
    
};

struct Vertex2 {
    glm::vec3 pos;
    glm::vec3 color;

};

const std::vector<Vertex2> vertices2 = {
    {{1.000000, -1.000000, -1.000000},{1.0f, 0.0f, 0.0f} },
    {{1.000000, -1.000000, 1.000000 } , { 1.0f, 1.0f, 1.0f }},
    {{-1.000000, -1.000000, 1.000000 } , { 1.0f, 0.0f, 1.0f }},
    {{-1.000000, -1.000000, -1.000000 } , { 1.0f, 1.0f, 0.0f }},
    {{1.000000, 1.000000, -1.000000 } , { 0.0f, 1.0f, 0.0f }},
    {{0.999999, 1.000000, 1.000001 } , { 1.0f, 0.0f, 1.0f }},
    {{ -1.000000, 1.000000, 1.000000}, { 0.0f, 0.0f, 1.0f }},
    {{- 1.000000, 1.000000, - 1.000000}, {1.0f, 1.0f, 1.0f}}
};


const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},
    {{-0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}}
 };


const std::vector<uint16_t> indicies = {
    0, 1, 2, 0, 2, 3
};

const std::vector<uint16_t> indicies2 = {
    4, 0, 3,
    4, 3, 7,
    2, 6, 7,
    2, 7, 3,
    1, 5 ,2,
    5, 6, 2,
    0, 4, 1,
    4, 5 ,1,
    4 ,7, 5,
    7, 6 ,5,
    0, 1, 2,
    0, 2, 3};


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

VkRenderPass renderPass;
VkPipeline graphicsPipeline;
VkPipelineLayout pipelineLayout;
VkDescriptorSetLayout descriptorLayout;

VkBuffer vertexBuffer;
VkDeviceMemory vertexBufferMemory;
VkBuffer indexBuffer;
VkDeviceMemory indexBufferMemory;
std::vector<VkBuffer> uniformBuffers;
std::vector<VkDeviceMemory> uniformBufferMemories;
std::vector<void*> uniformBufferMemoryMaps;

VkBuffer stagingBuffer;
VkDeviceMemory stagingBufferMemory;

VkSurfaceKHR surface;
VkSwapchainKHR swapchain;
VkExtent2D currentExtent;
VkSurfaceFormatKHR surfaceFormat;
VkPresentModeKHR presentMode;

VkCommandPool commandPool;
std::vector<VkCommandBuffer>  commandBuffer;

VkDescriptorPool descriptorPool;
std::vector<VkDescriptorSet> descriptorSets;



std::vector<VkSemaphore> renderSemaphores;
std::vector<VkSemaphore> imageAvailableSemaphores;
std::vector<VkFence> frameFences;

uint32_t currentFrame;

std::vector<VkImage> swapchainImages;
std::vector<VkImageView> swapchainImageViews;
std::vector<VkFramebuffer> swapchainBuffers;

struct uniformBufferObject MVP {};
float ypos = 0;
float xpos = 0;
float zpos = 0;

void initVulkan();
void createSurface();
void pickPhysicalDevice();
void createLogicalDevice();
void createSwapchain();
void createImageViews();
void createRenderPass();
void createGraphicsPipeline();
void createFrameBuffers();
void createVertexBuffer();
void createDescriptorPool();
void allocateDescriptorSet();
void createUniformBuffers();
void createDescriptorSetLayout();
void createIndexBuffer();
void createCommandPool();
void allocateCommandBuffer();
void recordCommandBuffer(VkCommandBuffer cbuffer, uint32_t imageIndex);
void createSyncObject();
void drawExample();
void updateUniformBuffer(uint32_t imageIndex);

// helpers

uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {

    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (int i = 0; i < memProperties.memoryTypeCount; i++) {
        if (typeFilter & (1 << i) && memProperties.memoryTypes[i].propertyFlags & properties) {
            return i;
        }

    }

}

void copyBuffers(VkBuffer stagingBuffer, VkBuffer dstBuffer, VkDeviceSize size);

void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

VkShaderModule createShaderModule(const std::vector<char>& code);

static std::vector<char> readFile(const std::string& filename);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){

    if (key == GLFW_KEY_A && action == GLFW_PRESS || key == GLFW_KEY_A && action == GLFW_REPEAT) {
        xpos += 0.1f;
    }else if (key == GLFW_KEY_D && action == GLFW_PRESS || key == GLFW_KEY_D && action == GLFW_REPEAT) {
        xpos -= 0.1f;
    }else if (key == GLFW_KEY_W && action == GLFW_PRESS || key == GLFW_KEY_W && action == GLFW_REPEAT) {
        ypos -= 0.1f;
    }else if (key == GLFW_KEY_S && action == GLFW_PRESS || key == GLFW_KEY_S && action == GLFW_REPEAT) {
        ypos += 0.1f;
    }

}


int main() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

    glfwSetKeyCallback(window, key_callback);

    MVP.model = glm::mat4(1.0f);
    MVP.view = glm::lookAt(glm::vec3(4.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    MVP.proj = glm::perspective(glm::radians(45.0f), ((float)800 / 600), 0.1f, 10.0f);

    MVP.proj[1][1] *= -1;
    
    
    initVulkan();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapchain();
    createImageViews();
    createRenderPass();
    createDescriptorPool();
    createDescriptorSetLayout();
    createGraphicsPipeline();
    createFrameBuffers();
    createCommandPool();
    createVertexBuffer();
    createIndexBuffer();
    createUniformBuffers();
    allocateDescriptorSet();
    allocateCommandBuffer();
    createSyncObject();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        drawExample();
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

    std::string deviceName = " ";

    for (VkPhysicalDevice pDevice : physicalDevices) {
        VkPhysicalDeviceProperties properties = {};
        vkGetPhysicalDeviceProperties(pDevice, &properties);
        std::cout << "--------------------------------" << std::endl;
        std::cout << "Device Name: " << properties.deviceName << std::endl;
        std::cout << "Device Type:" << properties.deviceType << std::endl;
        std::cout << "--------------------------------" << std::endl;
        if (properties.deviceType == 2) {
            physicalDevice = pDevice;
            deviceName = properties.deviceName;
        }
    
    }

    if (deviceName != " ") {
        std::cout << "Device selected: " << deviceName << std::endl;
    }
    else {
        physicalDevice = physicalDevices[0];
        std::cout << "Couldn't find discrete GPU, selecting first GPU instead!" << std::endl;
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

void createRenderPass() {
    VkAttachmentDescription colorAttachment{};
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;  
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    colorAttachment.format = surfaceFormat.format;


    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency subDepend{};
    subDepend.srcSubpass = VK_SUBPASS_EXTERNAL;
    subDepend.dstSubpass = 0;
    subDepend.srcStageMask =  VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT ;
    subDepend.srcAccessMask = 0;
    subDepend.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subDepend.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &subDepend;

    if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
        std::cerr << "Couldn't create render pass" << std::endl;
    }
}


void createGraphicsPipeline() {

    auto vertShaderCode = readFile("vert.spv");
    auto fragShaderCode = readFile("frag.spv");

    VkShaderModule vertShader = createShaderModule(vertShaderCode);
    VkShaderModule fragShader = createShaderModule(fragShaderCode);

    
    VkPipelineShaderStageCreateInfo vertInfo{};
    vertInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertInfo.module = vertShader;
    vertInfo.pName = "main";
    vertInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    
    
    VkPipelineShaderStageCreateInfo fragInfo{};
    fragInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragInfo.module = fragShader;
    fragInfo.pName = "main";
    fragInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertInfo, fragInfo };


    std::vector<VkDynamicState> dynamicState{
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicStates{};
    dynamicStates.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicStates.dynamicStateCount = static_cast<uint32_t>(dynamicState.size());
    dynamicStates.pDynamicStates = dynamicState.data();


    VkVertexInputBindingDescription bindingInfo{};
    bindingInfo.binding = 0;
    bindingInfo.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    bindingInfo.stride = sizeof(Vertex2);


    VkVertexInputAttributeDescription attributeInfos[2];
    attributeInfos[0].binding = 0;
    attributeInfos[0].location = 0;
    attributeInfos[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeInfos[0].offset = offsetof(Vertex2, pos);

    attributeInfos[1].binding = 0;
    attributeInfos[1].location = 1;
    attributeInfos[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeInfos[1].offset = offsetof(Vertex2, color);;
    

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexAttributeDescriptionCount = 2;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingInfo;
    vertexInputInfo.pVertexAttributeDescriptions = attributeInfos;

    VkPipelineInputAssemblyStateCreateInfo inputInfo{};
    inputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputInfo.primitiveRestartEnable = VK_FALSE;
    
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.height = currentExtent.height;
    viewport.width = currentExtent.width;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = { 0,0 };
    scissor.extent = currentExtent;

    VkPipelineViewportStateCreateInfo viewportInfo{};
    viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportInfo.viewportCount = 1;
    viewportInfo.pViewports = &viewport;
    viewportInfo.scissorCount = 1;
    viewportInfo.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizerInfo{};
    rasterizerInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizerInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizerInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizerInfo.depthClampEnable = VK_FALSE; 
    rasterizerInfo.rasterizerDiscardEnable = VK_FALSE;
    rasterizerInfo.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizerInfo.lineWidth = 1.0f;
    rasterizerInfo.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampleInfo{};
    multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleInfo.sampleShadingEnable = VK_FALSE;
    multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
    colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendInfo.logicOpEnable = VK_FALSE;
    colorBlendInfo.attachmentCount = 1;
    colorBlendInfo.pAttachments = &colorBlendAttachment;

    VkPipelineLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layoutInfo.setLayoutCount = 1;
    layoutInfo.pSetLayouts = &descriptorLayout;

    
    if (vkCreatePipelineLayout(device, &layoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
		std::cerr << "Couldn't create pipeline layout" << std::endl;
	}


    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputInfo;
    pipelineInfo.pColorBlendState = &colorBlendInfo;
    pipelineInfo.pRasterizationState = &rasterizerInfo;
    pipelineInfo.pDynamicState = &dynamicStates;
    pipelineInfo.pMultisampleState = &multisampleInfo;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.pViewportState = &viewportInfo;
    pipelineInfo.pDepthStencilState = nullptr;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1; // invalid index

    if (vkCreateGraphicsPipelines(device, nullptr, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
        std::cerr << "Graphics pipeline couldn't created!" << std::endl;
    }
    else {
        std::cout << "Graphics pipeline is created." << std::endl;
    }




}

void createFrameBuffers(){

    swapchainBuffers.resize(swapchainImageViews.size());

    bool flag = true;

    for (int i = 0; i < swapchainImageViews.size(); i++) {

        VkImageView attachments = {
            swapchainImageViews[i]
        };


        VkFramebufferCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        info.attachmentCount = 1;
        info.pAttachments = &attachments;
        info.renderPass = renderPass;
        info.height = currentExtent.height;
        info.width = currentExtent.width;
        info.layers = 1;
        
        if (vkCreateFramebuffer(device, &info, nullptr, &swapchainBuffers[i]) != VK_SUCCESS) {
            std::cerr << "Swapchain frame buffers couldn't created!" << std::endl;
            flag = false;
            break;
        } 


    }


    if (flag) {
        std::cout << "Swapchain framebuffers are created." << std::endl;
    }


}

void createVertexBuffer(){

    VkDeviceSize size = sizeof(vertices2[0]) * vertices2.size();
    createBuffer(size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT |VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        vertexBuffer, vertexBufferMemory);

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    createBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
        stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, size, 0, &data);
    memcpy(data, vertices2.data(), size);
    vkUnmapMemory(device, stagingBufferMemory);

    copyBuffers(stagingBuffer, vertexBuffer, size);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);


}

void createDescriptorPool(){


    VkDescriptorPoolSize poolSize{};
    poolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAME_ON_PROCESS);
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAME_ON_PROCESS);

    if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        std::cerr << "Descriptor pool couldn't created" << std::endl;

    }
    
    


}

void allocateDescriptorSet(){

    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAME_ON_PROCESS, descriptorLayout);

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAME_ON_PROCESS);
    allocInfo.pSetLayouts = layouts.data();
    descriptorSets.resize(MAX_FRAME_ON_PROCESS);
    
    if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        std::cerr << "Couldn't allocate descriptor sets!" << std::endl;
    }


    for (int i = 0; i < MAX_FRAME_ON_PROCESS; i++) {

        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(uniformBufferObject);

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstSet = descriptorSets[i];
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.pBufferInfo = &bufferInfo;

        vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);

    }

}

void createUniformBuffers(){

    uniformBuffers.resize(MAX_FRAME_ON_PROCESS);
    uniformBufferMemories.resize(MAX_FRAME_ON_PROCESS);
    uniformBufferMemoryMaps.resize(MAX_FRAME_ON_PROCESS);


    VkDeviceSize size = sizeof(glm::mat4) * 3;

    for (int i = 0; i < MAX_FRAME_ON_PROCESS; i++) {
       createBuffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
            uniformBuffers[i], uniformBufferMemories[i]);

       

       vkMapMemory(device, uniformBufferMemories[i], 0, size, 0, &uniformBufferMemoryMaps[i]);
       memcpy(uniformBufferMemoryMaps[i], &MVP, size);
    }
    

}

void createDescriptorSetLayout(){

    VkDescriptorSetLayoutBinding binding{};
    binding.binding = 0;
    binding.descriptorCount = 1;
    binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &binding;
    
    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorLayout) != VK_SUCCESS) {
        std::cerr << "Descriptor set layout couldn't created!" << std::endl;
    }

}

void createIndexBuffer(){

    VkDeviceSize size = sizeof(indicies2[0]) * indicies2.size();
    createBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, stagingBuffer, stagingBufferMemory);
    

    createBuffer(size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, size, 0, &data);
    memcpy(data, indicies2.data(), size);
    vkUnmapMemory(device, stagingBufferMemory);

    copyBuffers(stagingBuffer, indexBuffer, size);

    vkFreeMemory(device, stagingBufferMemory, nullptr);
    vkDestroyBuffer(device, stagingBuffer, nullptr);


}

void createCommandPool(){

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = 0;
    

    if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        std::cerr << "Command pool couldn't created!" << std::endl;
    }
    else {
        std::cout << "Command pool is created" << std::endl;
    }


}

void allocateCommandBuffer(){

    commandBuffer.resize(MAX_FRAME_ON_PROCESS);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = MAX_FRAME_ON_PROCESS;



    if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffer.data()) != VK_SUCCESS) {
        std::cerr << "Couldn't allocate command buffer" << std::endl;
    }
    else {
        std::cout << "Command buffer is allocated." << std::endl;
    }


}

void recordCommandBuffer(VkCommandBuffer cbuffer, uint32_t imageIndex){

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.pInheritanceInfo = nullptr;
    beginInfo.flags = 0;

    if (vkBeginCommandBuffer(cbuffer, &beginInfo) != VK_SUCCESS) {
        std::cerr << "Couldn't begin to record command buffer" << std::endl;
        return;
    }

    VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };

    VkRenderPassBeginInfo renderBeginInfo{};
    renderBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderBeginInfo.renderPass = renderPass;
    renderBeginInfo.framebuffer = swapchainBuffers[imageIndex];
    renderBeginInfo.renderArea.extent = currentExtent;
    renderBeginInfo.renderArea.offset = { 0, 0 };
    renderBeginInfo.clearValueCount = 1;
    renderBeginInfo.pClearValues = &clearColor;


    vkCmdBeginRenderPass(cbuffer, &renderBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(cbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

    vkCmdBindDescriptorSets(cbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[imageIndex], 0,  nullptr);

    VkBuffer buffers[] = { vertexBuffer };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(cbuffer, 0, 1, buffers, offsets);

    vkCmdBindIndexBuffer(cbuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);


    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.height = static_cast<float>(currentExtent.height);
    viewport.width = static_cast<float>(currentExtent.width);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 0.0f;
    vkCmdSetViewport(cbuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = currentExtent;
    vkCmdSetScissor(cbuffer, 0, 1, &scissor);

    vkCmdDrawIndexed(cbuffer, static_cast<uint32_t>(indicies2.size()), 1, 0, 0, 0);

    vkCmdEndRenderPass(cbuffer);

    if (vkEndCommandBuffer(cbuffer) != VK_SUCCESS) {
        std::cerr << "Couldn't finish the recording command buffer" << std::endl;
    }




}

void createSyncObject(){

    renderSemaphores.resize(MAX_FRAME_ON_PROCESS);
    imageAvailableSemaphores.resize(MAX_FRAME_ON_PROCESS);
    frameFences.resize(MAX_FRAME_ON_PROCESS);


    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    
    
    for (int i = 0; i < MAX_FRAME_ON_PROCESS; i++) {
        if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderSemaphores[i]) != VK_SUCCESS) {
            std::cerr << "Semaphore couldn't created" << std::endl;
        }
        if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS) {
            std::cerr << "Semaphore couldn't created" << std::endl;
        }

        if (vkCreateFence(device, &fenceInfo, nullptr, &frameFences[i]) != VK_SUCCESS) {
            std::cerr << "Fence couldn't created" << std::endl;
        }

    }

}

void drawExample(){

    uint32_t imageIndex = 0;
    

    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

    vkWaitForFences(device, 1, &frameFences[currentFrame], VK_TRUE, UINT64_MAX);

    vkResetFences(device, 1, &frameFences[currentFrame]);

    vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    updateUniformBuffer(imageIndex);

    vkResetCommandBuffer(commandBuffer[currentFrame], 0);

    recordCommandBuffer(commandBuffer[currentFrame], imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pCommandBuffers = &commandBuffer[currentFrame];
    submitInfo.commandBufferCount = 1;
    submitInfo.pSignalSemaphores = { &renderSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = { &imageAvailableSemaphores[currentFrame]};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitDstStageMask = waitStages;


    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, frameFences[currentFrame]) != VK_SUCCESS) {
        std::cerr << "Couldn't submit command buffer to graphics queue!" << std::endl;
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pSwapchains = { &swapchain };
    presentInfo.swapchainCount = 1;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pWaitSemaphores = { &renderSemaphores[currentFrame]};
    presentInfo.waitSemaphoreCount = 1;

    vkQueuePresentKHR(graphicsQueue, &presentInfo);


    currentFrame = (currentFrame + 1) % MAX_FRAME_ON_PROCESS;


}

void updateUniformBuffer(uint32_t imageIndex){

    MVP.model = glm::mat4(1.0f);
    MVP.view = glm::lookAt(glm::vec3(4.0f + xpos, 3.0f + ypos, 3.0f + zpos), glm::vec3(0.0f + xpos, 0.0f + ypos, 0.0f + zpos), glm::vec3(0.0f, 1.0f, 0.0f));
    MVP.proj = glm::perspective(glm::radians(45.0f), ((float)800 / 600), 0.1f, 10.0f);

    MVP.proj[1][1] *= -1;

    memcpy(uniformBufferMemoryMaps[imageIndex], &MVP, sizeof(MVP));
}

void copyBuffers(VkBuffer stagingBuffer, VkBuffer dstBuffer, VkDeviceSize size){

    VkCommandBuffer cbuffer;
    
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

    vkAllocateCommandBuffers(device, &allocInfo, &cbuffer);

    VkCommandBufferBeginInfo begInfo{};
    begInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(cbuffer, &begInfo);

    VkBufferCopy copyInfo{};
    copyInfo.dstOffset = 0;
    copyInfo.srcOffset = 0;
    copyInfo.size = size;

    vkCmdCopyBuffer(cbuffer, stagingBuffer, dstBuffer, 1, &copyInfo);
    vkEndCommandBuffer(cbuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pCommandBuffers = &cbuffer;
    submitInfo.commandBufferCount = 1;
    
    VkFence tempFence;
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    
    vkCreateFence(device, &fenceInfo, nullptr, &tempFence);


    vkQueueSubmit(graphicsQueue, 1, &submitInfo, tempFence);

    vkWaitForFences(device, 1, &tempFence, VK_TRUE, UINT64_MAX);



    vkDestroyFence(device, tempFence, nullptr);
    vkFreeCommandBuffers(device, commandPool, 1, &cbuffer);
}

void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, 
    VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory){
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.size = size;

    if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        std::cout << "Buffer couldn't created!" << std::endl;
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);


    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits,
        properties);
    allocInfo.allocationSize = memRequirements.size;

    if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        std::cerr << "Couldn't allocate memory for buffer!" << std::endl;
    }

    vkBindBufferMemory(device, buffer, bufferMemory, 0);


}

VkShaderModule createShaderModule(const std::vector<char>& code)
{
    VkShaderModule module;

    VkShaderModuleCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.codeSize = code.size();
    info.pCode = reinterpret_cast<const uint32_t*>(code.data());

    if (vkCreateShaderModule(device, &info, nullptr, &module) != VK_SUCCESS) {
        std::cerr << "Couldn't create shader module" << std::endl;
        return nullptr;
    }



    return module;
}


static std::vector<char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    
    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();
         
    return buffer;

}



