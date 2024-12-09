#include "EngineModel.h"

#include "Globals.h"
#include "EngineMesh.h"
#include "MathGeoLib.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "glew-2.1.0/include/GL/glew.h"

#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#define TINYGLTF_IMPLEMENTATION /* Only in one of the includes */
#include "tiny_gltf.h"

EngineModel::EngineModel()
{
	modelMatrix = float4x4::identity;

	minValues = float3::zero;
	maxValues = float3::zero;
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

	bool firstMesh = true;

	// TODO: INSTEAD OF ITERATING THROUGH THE MESHES, ITERATE THROUGH NODES THAT CONTAIN THE MESH POINTER AND ALSO CONTAINS THE TRANSFORMS FOR EACH MESH, PUT MODEL MATRIX INSIDE EACH MESH !!!!!!!
	for (tinygltf::Mesh& sourceMesh : model.meshes)
	{
		for (tinygltf::Primitive& primitive : sourceMesh.primitives)
		{
			EngineMesh* newMesh = new EngineMesh();
			newMesh->LoadVBO(model, sourceMesh, primitive);
			if (primitive.indices >= 0) newMesh->LoadEBO(model, sourceMesh, primitive);
			newMesh->CreateVAO();
			meshes.push_back(newMesh);

			float3 meshMaxValues = newMesh->GetMaximumPosition();
			float3 meshMinValues = newMesh->GetMinimumPosition();

			if (firstMesh)
			{
				firstMesh = false;
				maxValues = meshMaxValues;
				minValues = meshMinValues;
			}
			else
			{
				maxValues = float3(Max(maxValues.x, meshMaxValues.x), Max(maxValues.y, meshMaxValues.y), Max(maxValues.z, meshMaxValues.z));
				minValues = float3(Min(minValues.x, meshMinValues.x), Min(minValues.y, meshMinValues.y), Min(minValues.z, meshMinValues.z));
			}
		}
	}

	LoadMaterials(model, modelPath);
}

void EngineModel::LoadMaterials(const tinygltf::Model& sourceModel, const char* modelPath)
{
	for (const auto& srcMaterial : sourceModel.materials)
	{
		unsigned int textureId = 0;
		if (srcMaterial.pbrMetallicRoughness.baseColorTexture.index >= 0)
		{
			const tinygltf::Texture& texture = sourceModel.textures[srcMaterial.pbrMetallicRoughness.baseColorTexture.index];
			const tinygltf::Image& image = sourceModel.images[texture.source];

			// Removing file name from path to get the texture
			std::string stringPath = std::string(modelPath);
			int charsToIgnore = 0;
			auto pathIterator = stringPath.end();

			while (pathIterator-- != stringPath.begin() && *pathIterator != '/')
			{
				++charsToIgnore;
			}

			std::string texturePathString = stringPath.substr(0, stringPath.length() - charsToIgnore).append(image.uri);

			std::wstring wideUri = std::wstring(texturePathString.begin(), texturePathString.end());
			const wchar_t* texturePath = wideUri.c_str();

			textureId = App->GetTextureModule()->LoadTexture(texturePath);
		}
		textures.push_back(textureId);
	}
}

void EngineModel::SetModelMatrix(float4x4& newModelMatrix)
{
	modelMatrix = newModelMatrix;
}

void EngineModel::Render(int program, float4x4& projectionMatrix, float4x4& viewMatrix)
{
	for (EngineMesh* currentMesh : meshes)
	{
		int texturePostiion = textures.size() > 0 ? textures[0] : 0;
		currentMesh->Render(program, texturePostiion, projectionMatrix, viewMatrix, modelMatrix);
	}
}
