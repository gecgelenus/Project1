#include "dependencies.hpp"
#include "Window.hpp"
#include "Instance.hpp"
#include "Renderer.hpp"



 std::vector<Vertex> vertices2 = {
    {{1.000000, -1.000000, -1.000000},{0.0f, 1.0f, 0.0f}, {0}},
    {{1.000000, -1.000000, 1.000000 } , { 0.0f, 1.0f, 0.0f }, {0}},
    {{-1.000000, -1.000000, 1.000000 } , { 0.0f, 1.0f, 0.0f }, {0}},
    {{-1.000000, -1.000000, -1.000000 } , { 0.0f, 1.0f, 0.0f }, {0}},
    {{1.000000, 1.000000, -1.000000 } , { 0.0f, 1.0f, 0.0f }, {0}},
    {{0.999999, 1.000000, 1.000001 } , { 0.0f, 1.0f, 0.0f }, {0}},
    {{ -1.000000, 1.000000, 1.000000}, { 0.0f, 1.0f, 0.0f }, {0}},
    {{- 1.000000, 1.000000, - 1.000000}, {0.0f, 1.0f, 0.0f}, {0}}
};

 std::vector<Vertex> vertices3 = {
    {{1.000000, -1.000000 , -1.000000},{1.0f, 0.0f, 0.0f}, {0} },
    {{1.000000 , -1.000000 , 1.000000 } , { 1.0f, 0.0f, 0.0f }, {0}},
    {{-1.000000 , -1.000000, 1.000000 } , { 1.0f, 0.0f, 0.0f }, {0}},
    {{-1.000000 , -1.000000, -1.000000  } , { 1.0f, 0.0f, 0.0f }, {0}},
    {{1.000000 , 1.000000, -1.000000 } , { 1.0f, 0.0f, 0.0f }, {0}},
    {{0.999999 , 1.000000 , 1.000001  } , { 1.0f, 0.0f, 0.0f }, {0}},
    {{ -1.000000 , 1.000000 , 1.000000 }, { 1.0f, 0.0f, 0.0f }, {0}},
    {{-1.000000 , 1.000000 , -1.000000 }, {1.0f, 0.0f, 0.0f}, {0}}
 };


 std::vector<Vertex> vertices4 = {
    {{1.000000, -1.000000, -1.000000},{0.0f, 0.0f, 1.0f} , {0}},
    {{1.000000, -1.000000, 1.000000 } , { 0.0f, 0.0f, 1.0f }, {0}},
    {{-1.000000, -1.000000, 1.000000 } , { 0.0f, 0.0f, 1.0f }, {0}},
    {{-1.000000, -1.000000, -1.000000 } , { 0.0f, 0.0f, 1.0f }, {0}},
    {{1.000000, 1.000000, -1.000000 } , { 0.0f, 0.0f, 1.0f }, {0}},
    {{0.999999, 1.000000, 1.000001 } , { 0.0f, 0.0f, 1.0f }, {0}},
    {{ -1.000000, 1.000000, 1.000000}, { 0.0f, 0.0f, 1.0f }, {0}},
    {{-1.000000, 1.000000, -1.000000}, {0.0f, 0.0f, 1.0f}, {0}}
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

   


    Object object2(&instance, &window, "Object 2 - Red");
    object2.setVertices(vertices3);
    object2.setIndicies(indicies2);
    renderer.addObject(&object2);


    Object object3(&instance, &window, "Object 3 - Blue");
    object3.setVertices(vertices4);
    object3.setIndicies(indicies2);

    renderer.addObject(&object3);

    object3.move(glm::vec3(5.0f, 0.0f, 0.0f));
    object2.move(glm::vec3(-5.0f, 0.0f, 0.0f));


    Object object(&instance, &window, "Object 1 - Green");
    object.setVertices(vertices2);
    object.setIndicies(indicies2);
    renderer.addObject(&object);
    object.move(glm::vec3(0.0f, 5.0f, 0.0f));

    while (!glfwWindowShouldClose(window.getWindow())) {
        glfwPollEvents();
        
        renderer.drawFrame();
    }

    glfwDestroyWindow(window.getWindow());

    glfwTerminate();

    return 0;
}








