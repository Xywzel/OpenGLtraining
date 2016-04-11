#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(glm::vec3 dir, glm::vec3 diff, glm::vec3 spec, int index) {
	this->direction = dir;
	this->diffuse = diff;
	this->specular = spec;
	this->indexName = "directionalLights[" + std::to_string(index) + "].";
}

void DirectionalLight::update(Shader shader) {
	shader.use();
	glUniform3f(glGetUniformLocation(shader.program, (indexName + "direction").c_str()), this->direction.x, this->direction.y, this->direction.z);
	glUniform3f(glGetUniformLocation(shader.program, (indexName + "diffuse").c_str()), this->diffuse.x, this->diffuse.y, this->diffuse.z);
	glUniform3f(glGetUniformLocation(shader.program, (indexName + "specular").c_str()), this->specular.x, this->specular.y, this->specular.z);
}
