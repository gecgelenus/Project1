#include "Renderer.hpp"

Renderer::Renderer(Window* window, Instance* instance)
{
    
	this->window = window;
	this->instance = instance;
    
}

Renderer::~Renderer()
{
}

void Renderer::initialize()
{

    

    createImageViews();
	createRenderPass();
    createDescriptorPool();
    createDescriptorSetLayout();
	createGraphicsPipeline();
    createFramebuffers();
    createCommandPool();
	createUniformBuffers();
    createVertexBuffer();
    createIndexBuffer();
    createStagingBuffers();
	allocateDescriptorSet();
	createCommandBuffers();
	createSyncObjects();
    
}

void Renderer::createImageViews()
{
    this->swapchainImages = window->getSwapchainImages();

    swapchainImageViews.resize(swapchainImages.size());

    bool flag = true;

    for (int i = 0; i < swapchainImageViews.size(); i++) {
        VkImageViewCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        info.image = swapchainImages[i];
        info.format = window->getFormat();
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


        if (vkCreateImageView(instance->getDevice(), &info, nullptr, &swapchainImageViews[i]) != VK_SUCCESS) {
            std::cerr << "Image view couldn't created." << std::endl;
            flag = false;
            break;
        }

    }

    if (flag) {
        std::cout << "Image views are created." << std::endl;
    }



}

void Renderer::createRenderPass()
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    colorAttachment.format = window->getFormat();


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
    subDepend.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
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

    if (vkCreateRenderPass(instance->getDevice(), &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
        std::cerr << "Couldn't create render pass" << std::endl;
    }
}

void Renderer::createGraphicsPipeline()
{
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
    viewport.height = window->getExtent().height;
    viewport.width = window->getExtent().width;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = { 0,0 };
    scissor.extent = window->getExtent();

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


    if (vkCreatePipelineLayout(instance->getDevice(), &layoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
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

    if (vkCreateGraphicsPipelines(instance->getDevice(), nullptr, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
        std::cerr << "Graphics pipeline couldn't created!" << std::endl;
    }
    else {
        std::cout << "Graphics pipeline is created." << std::endl;
    }

}

void Renderer::createFramebuffers()
{
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
        info.height = window->getExtent().height;
        info.width= window->getExtent().width;
        info.layers = 1;

        if (vkCreateFramebuffer(instance->getDevice(), &info, nullptr, &swapchainBuffers[i]) != VK_SUCCESS) {
            std::cerr << "Swapchain frame buffers couldn't created!" << std::endl;
            flag = false;
            break;
        }


    }


    if (flag) {
        std::cout << "Swapchain framebuffers are created." << std::endl;
    }

}

void Renderer::createCommandPool()
{
	VkCommandPoolCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	info.queueFamilyIndex = 0;
	info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if (vkCreateCommandPool(instance->getDevice(), &info, nullptr, &commandPool) != VK_SUCCESS) {
		std::cerr << "Command pool couldn't created!" << std::endl;
	}
    else {
		std::cout << "Command pool is created." << std::endl;
	}
    instance->commandPool = commandPool;
}

void Renderer::createCommandBuffers()
{
    commandBuffers.resize(MAX_FRAME_ON_PROCESS);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = MAX_FRAME_ON_PROCESS;



    if (vkAllocateCommandBuffers(instance->getDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        std::cerr << "Couldn't allocate command buffer" << std::endl;
    }
    else {
        std::cout << "Command buffer is allocated." << std::endl;
    }

}

void Renderer::createDescriptorPool()
{

    VkDescriptorPoolSize poolSize{};
    poolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAME_ON_PROCESS)*2;
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAME_ON_PROCESS)*2;

    if (vkCreateDescriptorPool(instance->getDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        std::cerr << "Descriptor pool couldn't created" << std::endl;

    }
}

void Renderer::createDescriptorSetLayout()
{
    VkDescriptorSetLayoutBinding binding{};
    binding.binding = 0;
    binding.descriptorCount = 2;
    binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;


    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &binding;

    if (vkCreateDescriptorSetLayout(instance->getDevice(), &layoutInfo, nullptr, &descriptorLayout) != VK_SUCCESS) {
        std::cerr << "Descriptor set layout couldn't created!" << std::endl;
    }

}

void Renderer::allocateDescriptorSet()
{

    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAME_ON_PROCESS, descriptorLayout);

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAME_ON_PROCESS);
    allocInfo.pSetLayouts = layouts.data();
    descriptorSets.resize(MAX_FRAME_ON_PROCESS);

    if (vkAllocateDescriptorSets(instance->getDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        std::cerr << "Couldn't allocate descriptor sets!" << std::endl;
    }


    for (int i = 0; i < MAX_FRAME_ON_PROCESS; i++) {

        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(uniformBufferObject);

        VkDescriptorBufferInfo bufferInfo2{};
        bufferInfo2.buffer = uniformBuffers[i];
        bufferInfo2.offset = sizeof(uniformBufferObject);
        bufferInfo2.range = sizeof(uniformBufferObject);

        VkDescriptorBufferInfo infos[] = { bufferInfo , bufferInfo2 };

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.descriptorCount = 2;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstSet = descriptorSets[i];
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.pBufferInfo = infos;

        vkUpdateDescriptorSets(instance->getDevice(), 1, &descriptorWrite, 0, nullptr);
    }

}

void Renderer::createUniformBuffers()
{
    uniformBuffers.resize(MAX_FRAME_ON_PROCESS);
    uniformBufferMemories.resize(MAX_FRAME_ON_PROCESS);
    uniformBufferMemoryMaps.resize(MAX_FRAME_ON_PROCESS);


    VkDeviceSize size = sizeof(glm::mat4) * 6;

    for (int i = 0; i < MAX_FRAME_ON_PROCESS; i++) {
        createBuffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
            uniformBuffers[i], uniformBufferMemories[i]);



        vkMapMemory(instance->getDevice(), uniformBufferMemories[i], 0, size, 0, &uniformBufferMemoryMaps[i]);
        memcpy(uniformBufferMemoryMaps[i], window->MVP, size);
    }
}

void Renderer::createStagingBuffers()
{
    VkDeviceSize size = sizeof(Vertex2)  * PREALLOCATED_SIZE;

	createBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        		stagingBufferVertex, stagingBufferMemoryVertex);

	
    createBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBufferIndex, stagingBufferMemoryIndex);
}

void Renderer::updateVertexBuffer()
{
    void* data;
    vkMapMemory(instance->getDevice(), stagingBufferMemoryVertex, 0, sizeof(Vertex2) * vertices.size(), 0, &data);
    memcpy(data, vertices.data(), sizeof(Vertex2) * vertices.size());
	vkUnmapMemory(instance->getDevice(), stagingBufferMemoryVertex);

    copyBuffers(stagingBufferVertex, vertexBuffer, sizeof(Vertex2) * vertices.size());
}

void Renderer::updateIndexBuffer()
{
    void* data;
	vkMapMemory(instance->getDevice(), stagingBufferMemoryIndex, 0, sizeof(uint16_t) * indicies.size(), 0, &data);
	memcpy(data, indicies.data(), sizeof(uint16_t) * indicies.size());
	vkUnmapMemory(instance->getDevice(), stagingBufferMemoryIndex);

	copyBuffers(stagingBufferIndex, indexBuffer, sizeof(uint16_t) * indicies.size());
}

void Renderer::recordCommandBuffer(VkCommandBuffer cbuffer, uint32_t imageIndex)
{
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
    renderBeginInfo.renderArea.extent = window->getExtent();
    renderBeginInfo.renderArea.offset = { 0, 0 };
    renderBeginInfo.clearValueCount = 1;
    renderBeginInfo.pClearValues = &clearColor;


    vkCmdBeginRenderPass(cbuffer, &renderBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(cbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

    vkCmdBindDescriptorSets(cbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[imageIndex], 0, nullptr);

    VkBuffer vertexBuffers[] = {vertexBuffer};
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(cbuffer, 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(cbuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);


    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.height = static_cast<float>(window->getExtent().height);
    viewport.width = static_cast<float>(window->getExtent().width);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 0.0f;
    vkCmdSetViewport(cbuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = window->getExtent();
    vkCmdSetScissor(cbuffer, 0, 1, &scissor);

    vkCmdDrawIndexed(cbuffer, static_cast<uint32_t>(indicies.size()), 1, 0, 0, 0);

    vkCmdEndRenderPass(cbuffer);

    if (vkEndCommandBuffer(cbuffer) != VK_SUCCESS) {
        std::cerr << "Couldn't finish the recording command buffer" << std::endl;
    }
}

void Renderer::createSyncObjects()
{
    renderSemaphores.resize(MAX_FRAME_ON_PROCESS);
    imageAvailableSemaphores.resize(MAX_FRAME_ON_PROCESS);
    frameFences.resize(MAX_FRAME_ON_PROCESS);


    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;



    for (int i = 0; i < MAX_FRAME_ON_PROCESS; i++) {
        if (vkCreateSemaphore(instance->getDevice(), &semaphoreInfo, nullptr, &renderSemaphores[i]) != VK_SUCCESS) {
            std::cerr << "Semaphore couldn't created" << std::endl;
        }
        if (vkCreateSemaphore(instance->getDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS) {
            std::cerr << "Semaphore couldn't created" << std::endl;
        }

        if (vkCreateFence(instance->getDevice(), &fenceInfo, nullptr, &frameFences[i]) != VK_SUCCESS) {
            std::cerr << "Fence couldn't created" << std::endl;
        }

    }
}

void Renderer::drawFrame()
{
    uint32_t imageIndex = 0;


    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

    vkWaitForFences(instance->getDevice(), 1, &frameFences[currentFrame], VK_TRUE, UINT64_MAX);

    vkResetFences(instance->getDevice(), 1, &frameFences[currentFrame]);

    vkAcquireNextImageKHR(instance->getDevice(), window->getSwapchain(), UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    //updateUniformBuffer(imageIndex);

    vkResetCommandBuffer(commandBuffers[currentFrame], 0);

    recordCommandBuffer(commandBuffers[currentFrame], imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pCommandBuffers = &commandBuffers[currentFrame];
    submitInfo.commandBufferCount = 1;
    submitInfo.pSignalSemaphores = { &renderSemaphores[currentFrame] };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = { &imageAvailableSemaphores[currentFrame] };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitDstStageMask = waitStages;


    if (vkQueueSubmit(instance->getGraphicsQueue(), 1, &submitInfo, frameFences[currentFrame]) != VK_SUCCESS) {
        std::cerr << "Couldn't submit command buffer to graphics queue!" << std::endl;
    }

    VkSwapchainKHR swapchains[] = { window->getSwapchain() };

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pSwapchains = swapchains;
    presentInfo.swapchainCount = 1;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pWaitSemaphores = { &renderSemaphores[currentFrame] };
    presentInfo.waitSemaphoreCount = 1;

    vkQueuePresentKHR(instance->getGraphicsQueue(), &presentInfo);


    currentFrame = (currentFrame + 1) % MAX_FRAME_ON_PROCESS;
}

void Renderer::addObject(Object* object)
{
    object->objectIndex = objects.size();
    objects.push_back(object);
     
    

    object->vertexBias = vertices.size();
    object->indexOffset = indicies.size();

    for (int i = 0; i < object->indicies.size(); i++) {
		object->indicies[i] += object->vertexBias;
	}


    vertices.insert(vertices.end(), object->vertices.begin(), object->vertices.end() );
	indicies.insert(indicies.end(), object->indicies.begin(), object->indicies.end());
    

    

    updateVertexBuffer();
    updateIndexBuffer();

    std::cout << "Added object to renderer:" << object->getName() << std::endl;

}

void Renderer::removeObject(Object* object)
{
    for (int i = object->objectIndex+1; i < objects.size() - object->objectIndex; i++) {
        objects[i]->objectIndex--;
        objects[i]->vertexBias -= object->vertices.size();
        objects[i]->indexOffset -= object->indicies.size();
    }


    for (int i = object->indexOffset + object->indicies.size(); i <  indicies.size() ; i++) {
				indicies[i] -= object->vertices.size();
	}
	
    vertices.erase(vertices.begin() + object->vertexBias, vertices.begin() + object->vertexBias + object->vertices.size());
    indicies.erase(indicies.begin() + object->indexOffset, indicies.begin() + object->indexOffset + object->indicies.size());

    updateVertexBuffer();
    updateIndexBuffer();

    std::cout << "Removed object from renderer:" << object->getName() << std::endl;
    objects.erase(objects.begin() + object->objectIndex);

}

std::vector<char> Renderer::readFile(const std::string& filename) {
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

VkShaderModule Renderer::createShaderModule(const std::vector<char>& code)
{
    VkShaderModule module;

    VkShaderModuleCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.codeSize = code.size();
    info.pCode = reinterpret_cast<const uint32_t*>(code.data());

    if (vkCreateShaderModule(instance->getDevice(), &info, nullptr, &module) != VK_SUCCESS) {
        std::cerr << "Couldn't create shader module" << std::endl;
        return nullptr;
    }



    return module;
}


void Renderer::createVertexBuffer()
{
    VkDeviceSize size = sizeof(Vertex2) * PREALLOCATED_SIZE;
    createBuffer(size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        vertexBuffer, vertexBufferMemory);

    
}

void Renderer::createIndexBuffer()
{
    VkDeviceSize size = sizeof(uint16_t) * PREALLOCATED_SIZE;
    
    createBuffer(size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

    
}

void Renderer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory){
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.size = size;

    if (vkCreateBuffer(instance->getDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        std::cout << "Buffer couldn't created!" << std::endl;
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(instance->getDevice(), buffer, &memRequirements);


    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits,
        properties);
    allocInfo.allocationSize = memRequirements.size;

    if (vkAllocateMemory(instance->getDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        std::cerr << "Couldn't allocate memory for buffer!" << std::endl;
    }

    vkBindBufferMemory(instance->getDevice(), buffer, bufferMemory, 0);


}

uint32_t Renderer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {

    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(instance->getPhysicalDevice(), &memProperties);

    for (int i = 0; i < memProperties.memoryTypeCount; i++) {
        if (typeFilter & (1 << i) && memProperties.memoryTypes[i].propertyFlags & properties) {
            return i;
        }

    }

}


void Renderer::copyBuffers(VkBuffer stagingBuffer, VkBuffer dstBuffer, VkDeviceSize size) {

    VkCommandBuffer cbuffer;

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

    vkAllocateCommandBuffers(instance->getDevice(), &allocInfo, &cbuffer);

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

    vkCreateFence(instance->getDevice(), &fenceInfo, nullptr, &tempFence);


    vkQueueSubmit(instance->getGraphicsQueue(), 1, &submitInfo, tempFence);

    vkWaitForFences(instance->getDevice(), 1, &tempFence, VK_TRUE, UINT64_MAX);



    vkDestroyFence(instance->getDevice(), tempFence, nullptr);
    vkFreeCommandBuffers(instance->getDevice(), commandPool, 1, &cbuffer);
}

