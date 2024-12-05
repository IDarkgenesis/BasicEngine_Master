#pragma once

#include "Module.h"

class EngineModel;

class ModuleRenderMeshes : public Module
{
public:
	ModuleRenderMeshes();
	~ModuleRenderMeshes();

	bool Init() override;
	update_status Update(float deltaTime) override;
	bool CleanUp() override;
private:
	int program = -1;

	EngineModel* triangleNoIndices;
	EngineModel* triangleIndices;
};

