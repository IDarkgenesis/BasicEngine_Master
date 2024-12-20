#pragma once

#include <vector>
#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float2.h"

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


	void LoadAdditionalTexture(const char* texturePath);

	void Render(int program, float4x4& projectionMatrix, float4x4& viewMatrix);

	float3 GetMaximumValues() const { return maxValues; };
	float3 GetMinimumValues() const { return minValues; };

	int GetIndexCount() const { return indexCount; }
	int GetTotalLoadedTextures() const { return textures.size(); };
	int GetRenderTexture() const { return renderTexture; };
	void GetTextureSize(float2& outTextureSize);

	void SetRenderTexture(int texturePosition);

private:

	std::vector<EngineMesh*> meshes;
	std::vector<unsigned int> textures;
	std::vector<float2> textureInfo;

	int totalTriangles = 0;
	int renderTexture = -1;

	int indexCount = 0;

	float3 maxValues;
	float3 minValues;

	bool firstMesh = true;

	void LoadRecursive(const tinygltf::Model& sourceModel, const float4x4& parentModelMatrix, int currentNodePosition);
	void ClearVectors();
};

