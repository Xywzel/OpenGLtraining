#include "Camera.h"

#include <GLFW/glfw3.h>

// Default values
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVTY = 0.25f;
const GLfloat FOV = glm::radians(45.0f);
const GLfloat FOV_MAX = glm::radians(100.0f);
const GLfloat FOV_MIN = glm::radians(10.0f);
const GLfloat FOV_SENSITIVITY = 0.05f;
const glm::vec3 POSITION(0.0f, 0.0f, 0.0f);
const glm::vec3 FRONT(0.0f, 0.0f, 0.0f);
const glm::vec3 UP(0.0f, 1.0f, 0.0f);
const glm::vec3 RIGHT(1.0f, 0.0f, 0.0f);


Camera::Camera():
	position(POSITION),	front(FRONT), up(UP), right(RIGHT), worldUp(UP),
	yaw(YAW), pitch(PITCH),
	speed(SPEED), sensitivity(SENSITIVTY), fov(FOV) {
	this->updateVectors();
}

Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat yaw, GLfloat pitch):
	position(glm::vec3(posX, posY, posZ)), front(FRONT), up(UP), right(RIGHT), worldUp(UP),
	yaw(yaw), pitch(pitch),
	speed(SPEED), sensitivity(SENSITIVTY), fov(FOV) {
	this->updateVectors();
}

Camera::Camera(glm::vec3 pos, GLfloat yaw, GLfloat pitch):
	position(pos), front(FRONT), up(UP), right(RIGHT), worldUp(UP),
	yaw(yaw), pitch(pitch),
	speed(SPEED), sensitivity(SENSITIVTY), fov(FOV) {
	this->updateVectors();
}

glm::mat4 Camera::viewMatrix() {
	return glm::lookAt(this->position, this->position + this->front, this->up);
}

void Camera::update(bool keys[1024], glm::vec2 mouse, glm::vec2 scroll, GLfloat deltaTime) {
	processKeyboard(keys, deltaTime);
	processMouse(mouse);
	processScroll(scroll);
}

void Camera::processKeyboard(bool keys[1024], GLfloat deltaTime) {
	GLfloat velocity = this->speed * deltaTime;
	if (keys[GLFW_KEY_W]) this->position += this->front * velocity;
	if (keys[GLFW_KEY_S]) this->position -= this->front * velocity;
	if (keys[GLFW_KEY_A]) this->position -= this->right * velocity;
	if (keys[GLFW_KEY_D]) this->position += this->right * velocity;
}

void Camera::processMouse(glm::vec2 mouse) {
	mouse *= this->sensitivity;
	this->yaw += mouse.x;
	this->pitch -= mouse.y;
	if (this->pitch > 89.0f) this->pitch = 89.0f;
	if (this->pitch < -89.0f) this->pitch = -89.0f;
	this->updateVectors();
}

void Camera::processScroll(glm::vec2 scroll) {
	this->fov -= scroll.y * FOV_SENSITIVITY;
	if (this->fov < FOV_MIN) this->fov = FOV_MIN;
	if (this->fov > FOV_MAX) this->fov = FOV_MAX;
}

void Camera::updateVectors() {
	glm::vec3 front;
	front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	front.y = sin(glm::radians(this->pitch));
	front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	this->front = glm::normalize(front);
	this->right = glm::normalize(glm::cross(this->front, this->worldUp));
	this->up = glm::normalize(glm::cross(this->right, this->front));
}

