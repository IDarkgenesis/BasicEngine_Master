#pragma once

#include "Module.h"
#include "MathGeoLib/include/Math/float2.h"

class EngineModel;

class ModuleModelViewer : public Module
{
public:
		ModuleModelViewer();
		~ModuleModelViewer();

		bool Init() override;
		update_status Update(float deltaTime) override;
		bool CleanUp() override;

		float3 GetModelMaximumValues() const;
		float3 GetModelMinimumValues() const;

		void LoadModel(const char* path);
		void LoadTexture(const char* path);

		int GetModelLoadedTextures() const;
		int GetModelRenderTexture() const;
		int GetModelIndexCount() const;
		void GetModelTextureSize(float2& outTextureSize);

		void SetModelRenderTexture(int renderTexture);

private:
	unsigned int programTextures = 0;

	EngineModel* currentLoadedModel;
};

