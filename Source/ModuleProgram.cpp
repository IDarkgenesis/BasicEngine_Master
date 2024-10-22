#include "Globals.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "glew-2.1.0/include/GL/glew.h"

ModuleProgram::ModuleProgram()
{
}

ModuleProgram::~ModuleProgram()
{
}

char* ModuleProgram::LoadShaderSource(const char* shaderPath)
{
	char* data = nullptr;
	FILE* file = nullptr;

	fopen_s(&file, shaderPath, "rb");
	if (file)
	{
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		data = (char*)malloc(size + 1);
		fseek(file, 0, SEEK_SET);
		fread(data, 1, size, file);
		data[size] = 0;
		fclose(file);
	}

	return data;
}

unsigned ModuleProgram::CompileShader(unsigned shaderType, const char* source)
{

	unsigned shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, &source, 0);
	glCompileShader(shaderId);

	int compileSuccess = GL_FALSE;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileSuccess);

	if (compileSuccess == GL_FALSE)
	{
		int logLenght = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLenght);
		if (logLenght > 0)
		{
			int written = 0;
			char* info = (char*)malloc(logLenght);
			glGetShaderInfoLog(shaderId, logLenght, &written, info);
			LOG("Log Info: %s", info);
			free(info);
		}
	}

	return shaderId;
}

unsigned ModuleProgram::CreateProgram(unsigned vertexShader, unsigned fragmentShader)
{
	unsigned programId = glCreateProgram();
	glAttachShader(programId, vertexShader);
	glAttachShader(programId, fragmentShader);
	glLinkProgram(programId);

	int compileSuccess = GL_FALSE;
	glGetProgramiv(programId, GL_LINK_STATUS, &compileSuccess);

	if (compileSuccess == GL_FALSE)
	{
		int logLenght = 0;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLenght);

		if (logLenght > 0)
		{
			int written = 0;
			char* info = (char*)malloc(logLenght);
			glGetProgramInfoLog(programId, logLenght, &written, info);
			LOG("Program Log Info: %s", info);
			free(info);
		}
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	return programId;
}
