#pragma once

#include "Module.h"
#include <memory>

class EngineModel;

class ModuleRenderExercise : public Module
{
public:

	ModuleRenderExercise();
	~ModuleRenderExercise();

	bool Init() override;
	update_status Update(float deltaTime) override;
	bool CleanUp() override;

private:
	int program = -1;

	unsigned vbo = -1;
	unsigned baboonTexture = -1;

	//EngineModel* firstModel;
	std::unique_ptr<EngineModel> firstModel;
};

