#pragma once

#include <vector>
#include "MathGeoLib/include/Math/float4x4.h"

namespace tinygltf
{
	class Model;
}

class EngineMesh;

class EngineModel
{
public:
	EngineModel();
	~EngineModel();

	void Load(const char* modelPath);
	void LoadMaterials(const tinygltf::Model& sourceModel, const char* modelPath);

	void SetModelMatrix(float4x4& newModelMatrix);

	void Render(int program, float4x4& projectionMatrix, float4x4& viewMatrix);

private:

	std::vector<EngineMesh*> meshes;
	std::vector<unsigned int> textures;
	float4x4 modelMatrix;
};

