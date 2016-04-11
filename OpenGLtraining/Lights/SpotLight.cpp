#include "SpotLight.h"

#include <gl/glew.h>

SpotLight::SpotLight(glm::vec3 pos, glm::vec3 dir, glm::vec2 cut, glm::vec3 diff, glm::vec3 spec, int index) {
	this->position = pos;
	this->direction = dir;
	this->cutoff = cut;
	this->diffuse = diff;
	this->specular = spec;
	this->indexName = "spotLights["+ std::to_string(index) + "]";
}

void SpotLight::update(Shader shader) {
	shader.use();
	glUniform3f(glGetUniformLocation(shader.program, (indexName + "position").c_str()), this->position.x, this->position.y, this->position.z);
	glUniform3f(glGetUniformLocation(shader.program, (indexName + "falloff").c_str()), this->direction.x, this->direction.y, this->direction.z);
	glUniform2f(glGetUniformLocation(shader.program, (indexName + "falloff").c_str()), this->cutoff.x, this->cutoff.y);
	glUniform3f(glGetUniformLocation(shader.program, (indexName + "diffuse").c_str()), this->diffuse.x, this->diffuse.y, this->diffuse.z);
	glUniform3f(glGetUniformLocation(shader.program, (indexName + "specular").c_str()), this->specular.x, this->specular.y, this->specular.z);
}
