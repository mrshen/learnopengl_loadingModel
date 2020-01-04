#pragma once
#ifndef MY_MESH_H
#define MY_MESH_H

#include "my_shader.h"
#include <vector>
#include <string>

using namespace std;

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoords;
};

struct Texture
{
	unsigned int id;
	string type;
	string path;
};

//string TEXTURE_DIFFUSE = "diffuse";
//string TEXTURE_SPECULAR = "specular";

class MyMesh
{
public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	MyMesh(vector<Vertex> _vertices, vector<unsigned int> _indices, vector<Texture> _textures);
	void draw(MyShader shader);

private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};

#endif