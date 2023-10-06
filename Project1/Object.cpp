#include "Object.hpp"

Object::Object(Instance* instance, Window* window, const char* name)
{
    this->instance = instance;
    this->window = window;
    this->name = name;
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    vertexBias = 0;
    indexOffset = 0;
    objectIndex = 0;
}

Object::~Object()
{
}

void Object::setVertices(std::vector<Vertex2>& vertices)
{
    this->vertices = vertices;
    
}

void Object::setIndicies(std::vector<uint16_t>& indicies)
{
	this->indicies = indicies;
	
}

void Object::move(glm::vec3& position)
{
    this->position += position;
}
