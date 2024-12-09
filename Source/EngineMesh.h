#pragma once

#include "MathGeoLib/include/Math/float4x4.h"

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

	void Render(int program, int texturePosition, float4x4& projectionMatrix, float4x4& viewMatrix, float4x4& modelMatrix);

private:
	unsigned int vbo = 0;
	unsigned int ebo = 0;
	unsigned int vao = 0;
	unsigned int material = 0;

	int vertexCount = 0;
	int textureCoordCount = 0;
	int indexCount = 0;
};

