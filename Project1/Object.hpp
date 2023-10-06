#pragma once
#include "dependencies.hpp"
#include "Instance.hpp"
#include "Window.hpp"

class Object {


public:
	Object(Instance* instance, Window* window, const char* name);
	~Object();

	void setVertices(std::vector<Vertex2>& vertices);
	void setIndicies(std::vector<uint16_t>& indicies);
	void move(glm::vec3& position);

	std::string getName() { return name; }

	std::vector<Vertex2> vertices;
	std::vector<uint16_t> indicies;

	uint32_t vertexBias;
	uint32_t indexOffset;
	uint32_t objectIndex;

private:
	Instance* instance;
	Window* window;

	std::string name;
	
	glm::vec3 position;

	
	



};



