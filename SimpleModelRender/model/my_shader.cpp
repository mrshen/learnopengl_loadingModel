#include "my_shader.h"

using std::stringstream;
using std::ifstream;
using std::cout;
using std::endl;

MyShader::MyShader(const char* vertexShaderSrc, const char* fragmentShaderSrc)
{
	string vertexCode, fragmentCode;
	ifstream vertexFile, fragmentFile;

	vertexFile.exceptions(ifstream::failbit | ifstream::badbit);
	fragmentFile.exceptions(ifstream::failbit | ifstream::badbit);

	try
	{
		// open raw file
		vertexFile.open(vertexShaderSrc);
		fragmentFile.open(fragmentShaderSrc);

		stringstream vShaderStream, fShaderStream;
		vShaderStream << vertexFile.rdbuf();
		fShaderStream << fragmentFile.rdbuf();

		vertexFile.close();
		fragmentFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (ifstream::failure e)
	{
		cout << "Error: read shader file failed! >.<" << endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	GLuint vShaderID, fShaderID;
	initVertexShader(&vShaderID, vShaderCode);
	initFragmentShader(&fShaderID, fShaderCode);

	GLuint shaderProgramID;
	initShaderProgram(&shaderProgramID, vShaderID, fShaderID);
	glDeleteShader(vShaderID);
	glDeleteShader(fShaderID);

	this->setShaderProgramID(shaderProgramID);
}

/*MyShader::MyShader(const MyShader &_shader)
{
	*this = _shader;
}*/

void MyShader::use()
{
	glUseProgram(this->shaderProgramID);
}

void MyShader::setInt(const string& name, int val) const
{
	glUniform1i(glGetUniformLocation(this->shaderProgramID, name.c_str()), val);
}

void MyShader::setBool(const string& name, bool val) const
{
	glUniform1i(glGetUniformLocation(this->shaderProgramID, name.c_str()), (int)val);
}

void MyShader::setFloat(const string& name, float val) const
{
	glUniform1f(glGetUniformLocation(this->shaderProgramID, name.c_str()), val);
}

void MyShader::setMaxtrix(const string &name, float *val_ptr, int count, bool need_transpose) const
{
	glUniformMatrix4fv(glGetUniformLocation(this->shaderProgramID, name.c_str()), count, need_transpose, val_ptr);
}

void MyShader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(this->shaderProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void MyShader::setVec3(const std::string &name, const glm::vec3 &value) const
{
	glUniform3fv(glGetUniformLocation(this->shaderProgramID, name.c_str()), 1, &value[0]);
}

void MyShader::setVec3(const std::string &name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(this->shaderProgramID, name.c_str()), x, y, z);
}

// util functions
void initVertexShader(GLuint* vertextID, const char * &vertexSource)
{
	*vertextID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(*vertextID, 1, &vertexSource, NULL);
	glCompileShader(*vertextID);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(*vertextID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(*vertextID, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	}
}

void initFragmentShader(GLuint* fragmentID, const char * &fragmentSource)
{
	*fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(*fragmentID, 1, &fragmentSource, NULL);
	glCompileShader(*fragmentID);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(*fragmentID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(*fragmentID, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
	}
}

void initShaderProgram(GLuint* shaderProgramID, GLuint vertexShaderID, GLuint fragmentShaderID)
{
	*shaderProgramID = glCreateProgram();
	glAttachShader(*shaderProgramID, vertexShaderID);
	glAttachShader(*shaderProgramID, fragmentShaderID);
	glLinkProgram(*shaderProgramID);
}
