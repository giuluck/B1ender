#pragma once
#include "Utils.h"
#include <list>
#include <map>

/*
 * Manager for the Vertex/Fragment Shaders
 */
class Shader
{
public:
	Shader();
	~Shader();

	Shader* addShader(GLenum shaderType, string shaderFile);
	Shader* updateProgram();
	GLuint getProgramId();

	Shader* setVariableLocation(string name);
	Shader* setUniformFloat(string name, GLfloat value);
	Shader* setUniformVec4(string name, vec4 vector);
	Shader* setUniformVec3(string name, vec3 vector);
	Shader* setUniformMat4(string name, mat4 matrix);

private:
	GLuint programId;
	list<GLuint> shaders;
	map<string, GLuint> variables;

	char* readSource(string shaderFile);
	GLuint getLocationOrThrow(string name);
};

Shader::Shader()
{
	programId = glCreateProgram();
	glLinkProgram(programId);
	glUseProgram(programId);
}

inline Shader::~Shader()
{
	glDeleteProgram(programId);
}

inline Shader* Shader::addShader(GLenum shaderType, string shaderFile)
{
	GLuint shaderId = glCreateShader(shaderType);
	const char* shaderSource = readSource(shaderFile);
	glShaderSource(shaderId, 1, &shaderSource, NULL);
	glCompileShader(shaderId);
	delete shaderSource;

	shaders.push_back(shaderId);
	return this;
}

inline Shader* Shader::updateProgram()
{
	glDeleteProgram(programId);
	programId = glCreateProgram();
	for(GLuint shaderId : shaders) {
		glAttachShader(programId, shaderId);
	}
	glLinkProgram(programId);
	glUseProgram(programId);
	return this;
}

inline GLuint Shader::getProgramId()
{
	return this->programId;
}

inline Shader* Shader::setVariableLocation(string name)
{
	variables.insert({ name, glGetUniformLocation(getProgramId(), name.c_str()) });
	return this;
}

inline Shader* Shader::setUniformFloat(string name, GLfloat value)
{
	glUniform1f(getLocationOrThrow(name), value);
	return this;
}

inline Shader* Shader::setUniformVec3(string name, vec3 vector)
{
	glUniform3f(getLocationOrThrow(name), vector.x, vector.y, vector.z);
	return this;
}

inline Shader* Shader::setUniformVec4(string name, vec4 vector)
{
	glUniform4f(getLocationOrThrow(name), vector.x, vector.y, vector.z, vector.w);
	return this;
}

inline Shader* Shader::setUniformMat4(string name, mat4 matrix)
{
	glUniformMatrix4fv(getLocationOrThrow(name), 1, GL_FALSE, value_ptr(matrix));
	return this;
}

inline GLuint Shader::getLocationOrThrow(string name)
{
	map<string, GLuint>::iterator iterator = variables.find(name);
	return (iterator == variables.end()) ? throw "nonexistent variable" : iterator->second;
}

inline char* Shader::readSource(string shaderFile)
{
	FILE* file;
	char* buffer;
	long size;

	fopen_s(&file, shaderFile.c_str(), "rb");
	if (file == NULL)
		return NULL;

	fseek(file, 0L, SEEK_END);
	size = ftell(file);
	fseek(file, 0L, SEEK_SET);
	buffer = new char[size + 1];
	fread(buffer, 1, size, file);
	buffer[size] = '\0';
	fclose(file);

	return buffer;
}
