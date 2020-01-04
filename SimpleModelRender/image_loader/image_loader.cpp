#include "../common.h"
#include "stb_image.h"

using std::string;

GLuint loadTexture(const char *texturePath, const string &dirPath)
{
	string filename = string(texturePath);
	filename = dirPath + "/" + filename;

	GLuint texture;
	glGenTextures(1, &texture);

	int width, height, nrChannels;
	unsigned char* texData = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
	if (texData)
	{
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, texData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else
	{
		std::cout << "Error: FAILED TO LOAD TEXTURE [" << texturePath << "]." << std::endl;
	}
	stbi_image_free(texData);

	return texture;
}