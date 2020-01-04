#include "my_model.h"

void MyModel::Draw(MyShader shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].draw(shader);
	}
}

void MyModel::loadModel(string path)
{
	Assimp::Importer imporer;
	const aiScene *scn = imporer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scn || scn->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scn->mRootNode)
	{
		cout << "Error::ASSIMP::" << imporer.GetErrorString() << endl;
		return;
	}

	dirPath = path.substr(0, path.find_last_of('/'));
	processNode(scn->mRootNode, scn);
}

void MyModel::processNode(aiNode *node, const aiScene *scn)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scn->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scn));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scn);
	}
}

MyMesh MyModel::processMesh(aiMesh *mesh, const aiScene *scn)
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	// init mesh data
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 _vertor;
		
		_vertor.x = mesh->mVertices[i].x;
		_vertor.y = mesh->mVertices[i].y;
		_vertor.z = mesh->mVertices[i].z;
		vertex.position = _vertor;

		_vertor.x = mesh->mNormals[i].x;
		_vertor.y = mesh->mNormals[i].y;
		_vertor.z = mesh->mNormals[i].z;
		vertex.normal = _vertor;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 _vec2;
			_vec2.x = mesh->mTextureCoords[0][i].x;
			_vec2.y = mesh->mTextureCoords[0][i].y;
			vertex.texcoords = _vec2;
		}
		else
		{
			vertex.texcoords = glm::vec2(.0f, .0f);
		}
		vertices.push_back(vertex);
	}

	// init mesh indices of the triangle(vertices)
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// init material
	aiMaterial *mat = scn->mMaterials[mesh->mMaterialIndex];
	vector<Texture> diffuseMaps = loadMaterialTextures(mat, aiTextureType_DIFFUSE, "diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	vector<Texture> specularMaps = loadMaterialTextures(mat, aiTextureType_SPECULAR, "specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	return MyMesh(vertices, indices, textures);
}

vector<Texture> MyModel::loadMaterialTextures(aiMaterial *mat, aiTextureType texType, string typeName)
{
	vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(texType); i++)
	{
		aiString str;
		mat->GetTexture(texType, i, &str);

		bool loaded = false;
		for (unsigned int j = 0; j < textureCaches.size(); j++)
		{
			if (std::strcmp(textureCaches[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textureCaches[j]);
				loaded = true;
				break;
			}
		}

		if (!loaded)
		{
			Texture texture;
			texture.id = loadTexture(str.C_Str(), dirPath);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textureCaches.push_back(texture);
		}
	}

	return textures;
}
