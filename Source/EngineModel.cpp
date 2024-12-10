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

	for (tinygltf::Node currentNode : model.nodes)
	{
		if (currentNode.name != "RootNode")
		{
			// Creating basic model transform matrix based on node information
			float4x4 rotationX, rotationY, rotationZ, finalRotation;
			float3 translation, scale;

			finalRotation = float4x4::identity;
			translation = float3::zero; 
			scale = float3::one;

			if (currentNode.rotation.size() > 0)
			{
				rotationX = float4x4::RotateX((float)currentNode.rotation[0]);
				rotationY = float4x4::RotateX((float)currentNode.rotation[1]);
				rotationZ = float4x4::RotateX((float)currentNode.rotation[2]);

				finalRotation = rotationX * rotationY * rotationZ;
			}

			if (currentNode.translation.size() > 0)
			{
				translation = float3((float)currentNode.translation[0], (float)currentNode.translation[1], (float)currentNode.translation[2]);
			}

			if (currentNode.scale.size() > 0)
			{
				scale = float3((float)currentNode.scale[0], (float)currentNode.scale[1], (float)currentNode.scale[2]);
			}

			float4x4 basicModelMatrix = float4x4::FromTRS(
				translation,
				finalRotation,
				scale
				);

			for (tinygltf::Primitive& primitive : model.meshes[currentNode.mesh].primitives)
			{
				EngineMesh* newMesh = new EngineMesh();
				newMesh->LoadVBO(model, model.meshes[currentNode.mesh], primitive);
				if (primitive.indices >= 0) newMesh->LoadEBO(model, model.meshes[currentNode.mesh], primitive);
				newMesh->CreateVAO();

				float3 meshMaxValues = newMesh->GetMaximumPosition().Mul(scale);
				float3 meshMinValues = newMesh->GetMinimumPosition().Mul(scale);

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

				newMesh->SetBasicModelMatrix(basicModelMatrix);
				
				meshes.push_back(newMesh);
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

void EngineModel::Render(int program, float4x4& projectionMatrix, float4x4& viewMatrix)
{
	for (EngineMesh* currentMesh : meshes)
	{
		int texturePostiion = textures.size() > 0 ? textures[0] : 0;
		currentMesh->Render(program, texturePostiion, projectionMatrix, viewMatrix);
	}
}
