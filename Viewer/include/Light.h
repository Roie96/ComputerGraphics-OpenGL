#pragma once
#include <string>
#include <glm/glm.hpp>

class Light
{
public:
	Light(int id, glm::vec3 position);
	std::string GetName() { return name; }
	glm::vec3& GetPosition() { return position; }

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

private:
	std::string name;
	glm::vec3 position;
};
