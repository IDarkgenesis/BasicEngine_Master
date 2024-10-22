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
	int Program = -1;

	unsigned Vbo = -1;
};

