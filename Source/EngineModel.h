#pragma once

#include <vector>
#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Math/float3.h"

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

	float3 GetMaximumValues() const { return maxValues; };
	float3 GetMinimumValues() const { return minValues; };

private:

	std::vector<EngineMesh*> meshes;
	std::vector<unsigned int> textures;
	float4x4 modelMatrix;

	float3 maxValues;
	float3 minValues;

};

