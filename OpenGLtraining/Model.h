#pragma once

// C++ std
#include <vector>
#include <string>

// Libraries
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Local
#include "Mesh.h"
#include "Shader.h"

class Model {
public:
	Model(GLchar* path);
	void draw(Shader shader);
	std::vector<Texture> loadedTextures;
private:
	std::vector<Mesh> meshes;
	std::string directory;
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string name);
	GLuint textureFromFile(const char* path, std::string directory);
};