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

	std::vector<Vertex2> vertices;
	std::vector<uint16_t> indicies;

private:
	Instance* instance;
	Window* window;

	std::string name;
	
	glm::vec3 position;

	
	



};



