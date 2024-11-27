#pragma once

#include "Module.h"
#include "Globals.h"

class ModuleRenderExercise : public Module
{
public:

	ModuleRenderExercise();
	~ModuleRenderExercise();

	bool Init();
	update_status Update();
	bool CleanUp();

private:
	int program = -1;

	unsigned vbo = -1;
	unsigned baboonTexture = -1;
};

