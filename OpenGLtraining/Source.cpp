// C++ standard libs
#include <iostream>
#include <string>
#include <sstream>

// Open GL libraries
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Math helpers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>

//local deps
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Lights/PointLight.h"

const GLuint WIDTH = 800, HEIGHT = 600;
// Keyboard status
bool keys[1024];
// Mouse status
bool firstMouse = true;
glm::vec2 mousePos = glm::vec2(0.0f, 0.0f);
glm::vec2 mouseLast = glm::vec2(0.0f, 0.0f);
glm::vec2 mouseScroll = glm::vec2(0.0f, 0.0f);
// Frame rate
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), -90.0f, 0.0f);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS) keys[key] = true;
	else if (action == GLFW_RELEASE) keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		mouseLast = glm::vec2(xpos, ypos);
		firstMouse = false;
	}
	mousePos = glm::vec2(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	mouseScroll = glm::vec2(xoffset, yoffset);
}

void handle_keys(GLFWwindow* window) {
	if (keys[GLFW_KEY_ESCAPE]) glfwSetWindowShouldClose(window, GL_TRUE);
	
	if (keys[GLFW_KEY_KP_0]) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	if (keys[GLFW_KEY_KP_1]) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	if (keys[GLFW_KEY_KP_2]) glDisable(GL_DEPTH_TEST);
	else glEnable(GL_DEPTH_TEST);
}

int main() {
	std::cout << "We have a take off!" << std::endl;
	// Set openGL enviroment with GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Make Window and set context
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGLTests", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Callbacks
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Start GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cout << "GLEW not initialized succesfully. " << "Error: " << glewGetErrorString(err) << std::endl;
		return -1;
	}
	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST);
	
	//Shadders
	Shader shader = Shader::Shader("Shaders\\default.vertex", "Shaders\\default.frag");

	//Models
	Model modelThing("Resources\\suit\\nanosuit.obj");

	//Lights
	PointLight light1 = PointLight(glm::vec3(2.0f), glm::vec3(1.0f, 0.022, 0.0019), glm::vec3(1.0f), glm::vec3(1.0f), 0);
	light1.update(shader);

	//Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Time since last frame
		GLfloat currentFrame = (GLfloat) glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Events
		mouseScroll = glm::vec2(0.0f, 0.0f);
		glfwPollEvents();
		handle_keys(window);
		glm::vec2 mouseOffset = mousePos - mouseLast;
		mouseLast = mousePos;
		camera.update(keys, mouseOffset, mouseScroll, deltaTime);

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
	
		glm::mat4 view = camera.viewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glm::mat4 proj = glm::perspective(camera.fov, (float) WIDTH / (float) HEIGHT, NEAR_PLANE, FAR_PLANE);
		glUniformMatrix4fv(glGetUniformLocation(shader.program, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		modelThing.draw(shader);
		
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}