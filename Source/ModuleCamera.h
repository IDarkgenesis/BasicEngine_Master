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

	void SetFOV(float newHorizontalFov);
	void SetAspectRatio(float newAspectRatio);
	void SetPlaneDistances(float zNear, float zFar);
	void SetCameraPosition(float3 newPosition);
	void SetCameraFront(float3 newForward);
	void SetCameraUp(float3 newForward);
	void SetCameraTarget(float3 newTarget);

private:
	Frustum camera;

	float3 target;

	float cameraMoveSpeed = 2.0f;
};

