#pragma once
#ifndef MY_MODEL_H
#define MY_MODEL_H

#include "my_shader.h"
#include "my_mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

extern GLuint loadTexture(const char *, const string &);

class MyModel
{
public:
	MyModel(string const& path) { loadModel(path); }
	void Draw(MyShader shader);

	vector<Texture> textureCaches;
	vector<MyMesh> meshes;
	string dirPath;

private:
	void loadModel(string);
	void processNode(aiNode *, const aiScene *);
	MyMesh processMesh(aiMesh *, const aiScene *);

	vector<Texture> loadMaterialTextures(aiMaterial *, aiTextureType, string);
};

#endif