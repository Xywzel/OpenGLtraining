#include "Shader.h"

#include <string>
#include <fstream>

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath) {
	this->program = glCreateProgram();
	GLuint vertex, fragment, geometry;
	// Vertex
	const GLchar* vertexCode = fileToString(vertexPath).c_str();
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexCode, NULL);
	glCompileShader(vertex);
	checkErrors(vertex, "vertex");
	glAttachShader(this->program, vertex);
	// Fragment
	const GLchar* fragCode = fileToString(fragmentPath).c_str();
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragCode, NULL);
	glCompileShader(fragment);
	checkErrors(fragment, "fragment");
	glAttachShader(this->program, fragment);
	// Geometry
	if (geometryPath != nullptr) {
		const GLchar* geoCode = fileToString(geometryPath).c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &geoCode, NULL);
		glCompileShader(geometry);
		checkErrors(geometry, "geometry");
		glAttachShader(this->program, geometry);
	}
	glLinkProgram(this->program);
	checkErrors(this->program, "program");
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometryPath != nullptr) glDeleteShader(geometry);
}

void Shader::use() {
	glUseProgram(this->program);
}

std::string Shader::fileToString(const GLchar* path) {
	std::string code;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		shaderFile.open(path);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		code = shaderStream.str();
	} catch (std::ifstream::failure e) {
		std::cout << "ERRROR SHADER reading a file failed" << std::endl;
	}
	return code;
}

void Shader::checkErrors(GLuint shader, std::string type) {
	GLint success;
	GLchar info[1024];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (type != "program") {
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, info);
			std::cout << "ERROR SHADER filed to compile shadder of type " << type << "\n" << info << "\n" << std::endl;
		}
	} else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, info);
			std::cout << "ERROR SHADER program linking failed\n" << info << std::endl;
		}
	}
}
