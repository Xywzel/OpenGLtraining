// C++ standard libs
#include <iostream>
#include <string>
#include <sstream>

// Open GL libraries
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>

// Math helpers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//local deps
#include "Shader.h"
#include "Camera.h"

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
	//Shadders
	Shader shader = Shader::Shader("vertex.glsl", "frag.glsl");
	Shader lightShader = Shader::Shader("vertex.glsl", "lightFrag.glsl");

	// Scene
	glEnable(GL_DEPTH_TEST);
	GLfloat vertices[] = {
		// Positions           // Normals           // Texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	glm::vec3 locations[] = {
		glm::vec3( 0.0f, -5.0f, -0.57f),
		glm::vec3( 2.0f,  4.0f, -4.91f),
		glm::vec3(-1.5f, -3.0f, -10.52f),
		glm::vec3(-3.8f,  1.0f, -17.96f),
		glm::vec3( 2.4f, -2.8f, -16.11f),
		glm::vec3(-1.7f,  1.3f, -15.03f),
		glm::vec3( 1.3f, -1.4f, -2.19f),
		glm::vec3( 1.5f,  0.6f, -13.64f),
		glm::vec3( 1.3f,  3.3f, -12.39f),
		glm::vec3( 1.5f,  0.1f, -9.52f),
		glm::vec3( 1.5f,  0.2f, -3.5f),
		glm::vec3( 0.0f,  0.0f, -5.0f),
		glm::vec3( 2.0f,  5.0f, -6.5f),
		glm::vec3(-1.5f, -2.2f, -7.5f),
		glm::vec3(-3.8f, -2.0f, -8.3f),
		glm::vec3( 2.4f, -0.4f, -11.5f),
		glm::vec3(-1.7f,  3.0f, -14.5f),
		glm::vec3( 1.3f, -2.0f, -18.5f),
		glm::vec3( 1.5f,  1.1f, -19.5f),
		glm::vec3( 3.5f,  2.8f, -1.5f),
		glm::vec3(-2.2f,  3.4f, -12.91f),
		glm::vec3(-0.8f, -1.3f, -1.5f)
	};

	glm::vec3 lightLocations[] = {
		glm::vec3( 0.7f, -0.5f,  1.0f),
		glm::vec3( 2.3f, -3.3f, -6.0f),
		glm::vec3(-4.0f,  1.0f, -12.0f),
		glm::vec3( 0.0f,  3.0f, -3.0f)
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(VAO);
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) 0);
	glEnableVertexAttribArray(0);
	// Normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Lights
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	// Textures
	GLuint diffuseMap, specularMap, emissionMap;
	glGenTextures(1, &diffuseMap);
	int width, height;
	unsigned char* image;
	image = SOIL_load_image("container2.png", &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &specularMap);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	image = SOIL_load_image("container2_specular.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &emissionMap);
	glBindTexture(GL_TEXTURE_2D, emissionMap);
	image = SOIL_load_image("matrix.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	

	shader.use();
	// Materials
	glUniform1i(glGetUniformLocation(shader.program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(shader.program, "material.specular"), 1);
	glUniform1i(glGetUniformLocation(shader.program, "material.emission"), 2);
	glUniform1f(glGetUniformLocation(shader.program, "material.empower"), 0.005f);
	glUniform1f(glGetUniformLocation(shader.program, "material.shininess"), 64.0f);
	// DirLight
	glUniform3f(glGetUniformLocation(shader.program, "dirLight.direction"), 0.1f, -1.0f, 0.2f);
	glUniform3f(glGetUniformLocation(shader.program, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(shader.program, "dirLight.diffuse"), 0.2f, 0.2f, 0.2f);
	glUniform3f(glGetUniformLocation(shader.program, "dirLight.specular"), 0.2f, 0.2f, 0.2f);
	// Point lights
	for (int i = 0; i < 4; ++i) {
		std::stringstream ss;
		ss << "pointLights[" << i << "]";
		std::string point = ss.str();
		glUniform3f(glGetUniformLocation(shader.program, (point + ".position").c_str()), lightLocations[i].x, lightLocations[i].y, lightLocations[i].z);
		glUniform3f(glGetUniformLocation(shader.program, (point + ".falloff").c_str()), 1.0f, 0.12f, 0.045f);
		glUniform3f(glGetUniformLocation(shader.program, (point + ".diffuse").c_str()), (float) i * 0.33f, 1.0f - (float) i * 0.3f, 0.5f);
		glUniform3f(glGetUniformLocation(shader.program, (point + ".specular").c_str()), (float) i * 0.33f, 1.0f - (float) i * 0.3f, 0.8f);
	}

	//Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Time since last frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Events
		mouseScroll = glm::vec2(0.0f, 0.0f);
		glfwPollEvents();
		handle_keys(window);
		glm::vec2 mouseOffset = mousePos - mouseLast;
		mouseLast = mousePos;
		camera.update(keys, mouseOffset, mouseScroll, deltaTime);

		glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		// Shader parameters
		glUniform3f(glGetUniformLocation(shader.program, "spotLights[0].position"), camera.position.x, camera.position.y, camera.position.z);
		glUniform3f(glGetUniformLocation(shader.program, "spotLights[0].direction"), camera.front.x, camera.front.y, camera.front.z);
		glUniform2f(glGetUniformLocation(shader.program, "spotLights[0].cutoff"), glm::cos(glm::radians(12.0f)), glm::cos(glm::radians(14.0f)));
		glUniform3f(glGetUniformLocation(shader.program, "spotLights[0].diffuse"), 0.1f, 0.1f, 0.9f);
		glUniform3f(glGetUniformLocation(shader.program, "spotLights[0].specular"), 0.1f, 0.1f, 1.0f);
		

		glUniform3f(glGetUniformLocation(shader.program, "viewPos"), camera.position.x, camera.position.y, camera.position.z);

		GLuint modelLoc = glGetUniformLocation(shader.program, "model");
		GLuint viewLoc = glGetUniformLocation(shader.program, "view");
		GLuint projLoc = glGetUniformLocation(shader.program, "proj");

		glm::mat4 view = camera.viewMatrix();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 proj = glm::perspective(camera.fov, (float) WIDTH / (float) HEIGHT, NEAR_PLANE, FAR_PLANE);
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, emissionMap);

		glBindVertexArray(VAO);
		for (GLint i = 0; i < 20; ++i) {
			// Shader materials


			glm::mat4 model;
			model = glm::translate(model, locations[i]);
			model = glm::rotate(model, (GLfloat) glfwGetTime() * glm::radians(10.0f * (GLfloat) ((i % 6) + 1)), glm::vec3(0.05f * (GLfloat) i, 1.0f - 0.05f * (GLfloat) i, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		lightShader.use();
		modelLoc = glGetUniformLocation(lightShader.program, "model");
		viewLoc = glGetUniformLocation(lightShader.program, "view");
		projLoc = glGetUniformLocation(lightShader.program, "proj");
		GLuint lcLoc = glGetUniformLocation(lightShader.program, "lightColor");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
		for (GLint i = 0; i < 4; ++i) {
			glm::mat4 model;
			model = glm::translate(model, lightLocations[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			glUniform3f(lcLoc, (float) i * 0.33f, 1.0f - (float) i * 0.3f, 0.5f);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(lightVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}