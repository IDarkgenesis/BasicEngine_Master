#pragma once

#include"Module.h"

class ModuleProgram : public Module
{
public:
	ModuleProgram();

	~ModuleProgram();

	char* LoadShaderSource(const char* shaderPath);

	unsigned CompileShader(unsigned shaderType, const char* source);

	unsigned CreateProgram(unsigned vertexShader, unsigned fragmentShader);

};

