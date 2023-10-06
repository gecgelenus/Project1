#include "dependencies.hpp"
#include "Window.hpp"
#include "Instance.hpp"
#include "Renderer.hpp"



 std::vector<Vertex2> vertices2 = {
    {{1.000000, -1.000000, -1.000000},{0.0f, 1.0f, 0.0f} },
    {{1.000000, -1.000000, 1.000000 } , { 0.0f, 1.0f, 0.0f }},
    {{-1.000000, -1.000000, 1.000000 } , { 0.0f, 1.0f, 0.0f }},
    {{-1.000000, -1.000000, -1.000000 } , { 0.0f, 1.0f, 0.0f }},
    {{1.000000, 1.000000, -1.000000 } , { 0.0f, 1.0f, 0.0f }},
    {{0.999999, 1.000000, 1.000001 } , { 0.0f, 1.0f, 0.0f }},
    {{ -1.000000, 1.000000, 1.000000}, { 0.0f, 1.0f, 0.0f }},
    {{- 1.000000, 1.000000, - 1.000000}, {0.0f, 1.0f, 0.0f}}
};

 std::vector<Vertex2> vertices3 = {
    {{1.000000, -1.000000 , -1.000000},{1.0f, 0.0f, 0.0f} },
    {{1.000000 , -1.000000 , 1.000000 } , { 1.0f, 0.0f, 0.0f }},
    {{-1.000000 , -1.000000, 1.000000 } , { 1.0f, 0.0f, 0.0f }},
    {{-1.000000 , -1.000000, -1.000000  } , { 1.0f, 0.0f, 0.0f }},
    {{1.000000 , 1.000000, -1.000000 } , { 1.0f, 0.0f, 0.0f }},
    {{0.999999 , 1.000000 , 1.000001  } , { 1.0f, 0.0f, 0.0f }},
    {{ -1.000000 , 1.000000 , 1.000000 }, { 1.0f, 0.0f, 0.0f }},
    {{-1.000000 , 1.000000 , -1.000000 }, {1.0f, 0.0f, 0.0f}}
 };


 std::vector<Vertex2> vertices4 = {
    {{1.000000, -1.000000, -1.000000},{0.0f, 0.0f, 1.0f} },
    {{1.000000, -1.000000, 1.000000 } , { 0.0f, 0.0f, 1.0f }},
    {{-1.000000, -1.000000, 1.000000 } , { 0.0f, 0.0f, 1.0f }},
    {{-1.000000, -1.000000, -1.000000 } , { 0.0f, 0.0f, 1.0f }},
    {{1.000000, 1.000000, -1.000000 } , { 0.0f, 0.0f, 1.0f }},
    {{0.999999, 1.000000, 1.000001 } , { 0.0f, 0.0f, 1.0f }},
    {{ -1.000000, 1.000000, 1.000000}, { 0.0f, 0.0f, 1.0f }},
    {{-1.000000, 1.000000, -1.000000}, {0.0f, 0.0f, 1.0f}}
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

    Object object(&instance, &window, "Object 1 - Green");
    object.setVertices(vertices2);
    object.setIndicies(indicies2);
    renderer.addObject(&object);


    Object object2(&instance, &window, "Object 2 - Red");
    object2.setVertices(vertices3);
    object2.setIndicies(indicies2);
    renderer.addObject(&object2);

    renderer.removeObject(&object);


    Object object3(&instance, &window, "Object 3 - Blue");
    object3.setVertices(vertices4);
    object3.setIndicies(indicies2);
    renderer.addObject(&object3);

  
    

    while (!glfwWindowShouldClose(window.getWindow())) {
        glfwPollEvents();
        renderer.drawFrame();
    }

    glfwDestroyWindow(window.getWindow());

    glfwTerminate();

    return 0;
}








