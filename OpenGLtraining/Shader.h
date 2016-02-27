#ifndef SHADDER_H
#define SHADDER_H

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
	public:
	GLuint program;
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath = nullptr);
	void use();

	private:
	std::string fileToString(const GLchar* path);
	void checkErrors(GLuint shader, std::string type);

};

#endif // SHADDER_H
