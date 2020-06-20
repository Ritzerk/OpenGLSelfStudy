#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

//Creating a struct so that we can return the two separate shaders from ParseShader.
struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader 
{
private:
	ShaderProgramSource vfSource;
	unsigned int vertexShader, fragmentShader;
public:

	unsigned int programID;

	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	void readShaderFiles(const char* vertexPath, const char* fragmentPath);
	void compileShaders();
	void createProgram();

	void use();

	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setMatrix4fv(const std::string& name, float* value) const;

};

#endif
