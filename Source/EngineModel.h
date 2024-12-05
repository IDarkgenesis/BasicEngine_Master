#pragma once

#include <vector>
#include "MathGeoLib/include/Math/float4x4.h"

class EngineMesh;

class EngineModel
{
public:
	EngineModel();
	~EngineModel();

	void Load(const char* modelPath);

	void Render(int program);

private:

	std::vector<EngineMesh*> meshes;
};

