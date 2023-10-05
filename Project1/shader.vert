#version 450

layout(set = 0, binding = 0) uniform uniformBufferObject{
	mat4 model;
	mat4 view;
	mat4 proj;
}MVP[2];



layout(location = 0) out vec3 fragColor;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;


void main() {
	gl_Position = MVP[0].proj * MVP[0].view * MVP[int(inColor.x)].model * vec4(inPosition, 1.0);
	fragColor = inColor;
}