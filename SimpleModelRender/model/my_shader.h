#pragma once
#ifndef MY_SHADER_H
#define MY_SHADER_H

#include "../common.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>

using std::string;

void initVertexShader(GLuint*, const char * &);
void initFragmentShader(GLuint*, const char * &);
void initShaderProgram(GLuint*, GLuint, GLuint);

class MyShader
{
public:
	MyShader(const char*, const char*);
	//MyShader(const MyShader &);
	void use();

	// uniform utils
	void setInt(const string&, int) const;
	void setBool(const string&, bool) const;
	void setFloat(const string&, float) const;
	void setMaxtrix(const string&, float*, int count = 1, bool need_transpose = GL_FALSE) const;
	void setMat4(const std::string &, const glm::mat4 &) const;
	void setVec3(const string &, const glm::vec3 &value) const;
	void setVec3(const string &, float x, float y, float z) const;

	GLuint getShaderProgramID()
	{
		return this->shaderProgramID;
	}

	void setShaderProgramID(GLuint val)
	{
		this->shaderProgramID = val;
	}

private:
	GLuint shaderProgramID;
};

#endif // !MY_SHADER_H
