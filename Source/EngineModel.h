#pragma once

#include <vector>
#include <memory>

#include "EngineMesh.h"

class EngineModel
{
public:
	EngineModel();
	~EngineModel();

	void Load(const char* modelPath);

private:

	std::vector<EngineMesh> meshes;
};

