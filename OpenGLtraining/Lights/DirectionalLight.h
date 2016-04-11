#pragma once
// C++ Std
#include <string>
// OpenGL helpers
#include <glm/glm.hpp>
// Local deps
#include "../Shader.h"

class DirectionalLight {
public:
	DirectionalLight(glm::vec3 dir, glm::vec3 diff, glm::vec3 spec, int index);
	void update(Shader shader);
private:
	glm::vec3 direction;
	glm::vec3 diffuse;
	glm::vec3 specular;
	std::string indexName;
};