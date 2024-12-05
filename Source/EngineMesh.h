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

	void Render(int program);

private:
	unsigned int vbo = 0;
	unsigned int ebo = 0;
	unsigned int material = 0;

	int vertexCount = 0; 
	int indexCount = 0;
};

