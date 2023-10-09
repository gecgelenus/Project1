#version 450

layout(set = 0, binding = 0) uniform uniformBufferObject{
	mat4 model[1000];
	mat4 view;
	mat4 proj;
}MVP;



layout(location = 0) out vec3 fragColor;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in uint objectIndex;

void main() {
	gl_Position = MVP.proj * MVP.view * MVP.model[objectIndex] * vec4(inPosition, 1.0);
	fragColor = inColor;
}