#pragma once

#include "Module.h"
#include "MathGeoLib/include/Math/float3.h"

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

private:
	unsigned int programTextures = 0;

	EngineModel* currentLoadedModel;
};

