#include "PointLight.h"

#include <gl/glew.h>


PointLight::PointLight(glm::vec3 pos, glm::vec3 fall, glm::vec3 diff, glm::vec3 spec, int index){
	this->position = pos;
	this->falloff = fall;
	this->diffuse = diff;
	this->specular = spec;
	this->indexName = "pointLights[" + std::to_string(index) + "].";
}

void PointLight::update(Shader shader) {
	shader.use();
	glUniform3f(glGetUniformLocation(shader.program, (indexName + "position").c_str()), this->position.x, this->position.y, this->position.z);
	glUniform3f(glGetUniformLocation(shader.program, (indexName + "falloff").c_str()), this->falloff.x, this->falloff.y, this->falloff.z);
	glUniform3f(glGetUniformLocation(shader.program, (indexName + "diffuse").c_str()), this->diffuse.x, this->diffuse.y, this->diffuse.z);
	glUniform3f(glGetUniformLocation(shader.program, (indexName + "specular").c_str()), this->specular.x, this->specular.y, this->specular.z);
}
