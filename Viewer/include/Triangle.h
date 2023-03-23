#pragma once
#include <glm/glm.hpp>
class Triangle
{
public:
	Triangle(glm::vec3 v1,glm::vec3 v2,glm::vec3 v3) {
		this->v1 = v1,
		this->v2 = v2,
		this->v3 = v3;
	}

	glm::ivec2 getMinPoint() { return glm::ivec2(glm::min(v1, glm::min(v2, v3))); }
	glm::ivec2 getMaxPoint() { return glm::ivec2(glm::max(v1, glm::max(v2, v3))); }

	glm::vec3 v1;
	glm::vec3 v2;
	glm::vec3 v3;
};