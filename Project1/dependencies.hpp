#pragma once

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

#define PREALLOCATED_SIZE 1000000
#define MAX_OBJECTS 1000


struct uniformBufferObject {
    glm::mat4 model[MAX_OBJECTS];
    glm::mat4 view;
    glm::mat4 proj;
};



struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::uint16 objectIndex;
};


