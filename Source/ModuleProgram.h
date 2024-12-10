#pragma once

#include"Module.h"

class ModuleProgram : public Module
{
public:
	ModuleProgram();

	~ModuleProgram();

	unsigned int LoadShaders(const char* vertexPath, const char* fragmentPath);

	char* LoadShaderSource(const char* shaderPath);

	unsigned int CompileShader(unsigned shaderType, const char* source);

	unsigned int CreateProgram(unsigned vertexShader, unsigned fragmentShader);

};

