#include "EngineModel.h"

#include "Globals.h"
#include "EngineMesh.h"
#include "MathGeoLib.h"

#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#define TINYGLTF_IMPLEMENTATION /* Only in one of the includes */
#include "tiny_gltf.h"

EngineModel::EngineModel()
{
}

EngineModel::~EngineModel()
{
	for (auto it : meshes)
	{
		delete it;
	}
}

void EngineModel::Load(const char* modelPath)
{
	GLOG("Loading: %s", modelPath);

	tinygltf::TinyGLTF gltfContext;
	tinygltf::Model model;
	std::string error, warning;

	bool loadOk = gltfContext.LoadASCIIFromFile(&model, &error, &warning, modelPath);

	if (!loadOk)
	{
		GLOG("Error loading %s: %s", modelPath, error.c_str());
	}

	for (tinygltf::Mesh& sourceMesh : model.meshes)
	{
		for (tinygltf::Primitive& primitive : sourceMesh.primitives)
		{
			EngineMesh* newMesh = new EngineMesh();
			newMesh->LoadVBO(model, sourceMesh, primitive);
			if (primitive.indices >= 0) newMesh->LoadEBO(model, sourceMesh, primitive);
			meshes.push_back(newMesh);
		}
	}
}

void EngineModel::Render(int program)
{
	for (EngineMesh* currentMesh : meshes)
	{
		currentMesh->Render(program);
	}
}
