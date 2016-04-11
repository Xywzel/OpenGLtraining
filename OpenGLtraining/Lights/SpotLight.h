#pragma once
// C++ Std
#include <string>
// OpenGL helpers
#include <glm/glm.hpp>
// Local deps
#include "../Shader.h"

class SpotLight {
public:
	SpotLight(glm::vec3 pos, glm::vec3 dir, glm::vec2 cut, glm::vec3 diff, glm::vec3 spec, int index);
	void update(Shader shader);
private:
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec2 cutoff;
	glm::vec3 diffuse;
	glm::vec3 specular;
	std::string indexName;
};