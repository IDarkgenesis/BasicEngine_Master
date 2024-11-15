#pragma once

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/include/Geometry/Frustum.h"

class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	bool Init();
	update_status Update();
	bool CleanUp();

	static float4x4 GetLookAtMatrix(float3 cameraPosition, float3 targetPosition, float3 upVector);
	
	float4x4 GetLookAtMatrix(float3 targetPosition);

	float4x4 GetLookAtMatrix();

	float4x4 GetViewMatrix();
	float4x4 GetProjectionMatrix();

private:
	Frustum camera;

	float3 cameraPosition;

	float3 front;

	float3 up;

	float3 target;

	float verticalFov;
	float horizontalFov;

	FrustumType frustumType;

	float cameraMoveSpeed = 2.0f;
};

