#include "dependencies.hpp"
#include "Window.hpp"
#include "Instance.hpp"
#include "Renderer.hpp"



 std::vector<Vertex2> vertices2 = {
    {{1.000000, -1.000000, -1.000000},{0.0f, 0.0f, 0.0f} },
    {{1.000000, -1.000000, 1.000000 } , { 0.0f, 1.0f, 1.0f }},
    {{-1.000000, -1.000000, 1.000000 } , { 0.0f, 0.0f, 1.0f }},
    {{-1.000000, -1.000000, -1.000000 } , { 0.0f, 1.0f, 0.0f }},
    {{1.000000, 1.000000, -1.000000 } , { 0.0f, 1.0f, 0.0f }},
    {{0.999999, 1.000000, 1.000001 } , { 0.0f, 0.0f, 1.0f }},
    {{ -1.000000, 1.000000, 1.000000}, { 0.0f, 0.0f, 1.0f }},
    {{- 1.000000, 1.000000, - 1.000000}, {0.0f, 1.0f, 1.0f}}
};

 std::vector<Vertex2> vertices3 = {
    {{1.000000, -1.000000 , -1.000000},{1.0f, 0.0f, 0.0f} },
    {{1.000000 , -1.000000 , 1.000000 } , { 1.0f, 1.0f, 1.0f }},
    {{-1.000000 , -1.000000, 1.000000 } , { 1.0f, 0.0f, 1.0f }},
    {{-1.000000 , -1.000000, -1.000000  } , { 1.0f, 1.0f, 0.0f }},
    {{1.000000 , 1.000000, -1.000000 } , { 1.0f, 1.0f, 0.0f }},
    {{0.999999 , 1.000000 , 1.000001  } , { 1.0f, 0.0f, 1.0f }},
    {{ -1.000000 , 1.000000 , 1.000000 }, { 1.0f, 0.0f, 1.0f }},
    {{-1.000000 , 1.000000 , -1.000000 }, {1.0f, 1.0f, 1.0f}}
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

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

 std::vector<uint16_t> indicies2 = {
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
















int main() {

    
    Window window(800, 600, "Vulkan");
    window.initialize();
    window.createWindow();
    
    Instance instance;
    window.instance = &instance;
    instance.initialize();


    window.createSurface();

   
    
    
    instance.pickPhysicalDevice();
    instance.createLogicalDevice();
    window.createSwapchain();


    Renderer renderer(&window, &instance);
    renderer.initialize();

    Object object(&instance, &window, "Triangle");
    object.setVertices(vertices2);
    object.setIndicies(indicies2);
    renderer.addObject(&object);

    std::vector<uint16_t> indicies3;

    for (uint16_t a: indicies2) {
        indicies3.push_back(a + vertices2.size());
    }

    Object object2(&instance, &window, "Triangle");
    object2.setVertices(vertices3);
    object2.setIndicies(indicies3);
    renderer.addObject(&object2);
    
    for (int i = 0; i < MAX_FRAME_ON_PROCESS; i++) {
        renderer.recordCommandBuffer(renderer.commandBuffers[i], i);
    }
    
    

    while (!glfwWindowShouldClose(window.getWindow())) {
        glfwPollEvents();
        renderer.drawFrame();
    }

    glfwDestroyWindow(window.getWindow());

    glfwTerminate();

    return 0;
}








