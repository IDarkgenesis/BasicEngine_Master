#pragma once

#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Math/float3.h"

namespace tinygltf
{
	class Model;
	struct Mesh;
	struct Primitive;
}

class EngineMesh
{
public:
	EngineMesh();
	~EngineMesh();

	void LoadVBO(const tinygltf::Model& inModel, const tinygltf::Mesh& inMesh, const tinygltf::Primitive& inPrimitive);
	void LoadEBO(const tinygltf::Model& inModel, const tinygltf::Mesh& inMesh, const tinygltf::Primitive& inPrimitive);
	void CreateVAO();

	float3 GetMaximumPosition() const { return maximumPosition; } 
	float3 GetMinimumPosition() const { return minimumPosition; }

	void SetBasicModelMatrix(float4x4& newModelMatrix);

	void Render(int program, int texturePosition, float4x4& projectionMatrix, float4x4& viewMatrix);

private:
	unsigned int vbo = 0;
	unsigned int ebo = 0;
	unsigned int vao = 0;
	unsigned int material = 0;

	int vertexCount = 0;
	int textureCoordCount = 0;
	int indexCount = 0;

	float4x4 basicModelMatrix;

	float3 maximumPosition;
	float3 minimumPosition;
};

