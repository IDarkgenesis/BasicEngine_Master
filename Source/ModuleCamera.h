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

	static float4x4 GetLookAtMatrix(const float3& cameraPosition, const float3& targetPosition, const float3& upVector);
	float4x4 GetLookAtMatrix(const float3& targetPosition) const;
	float4x4 GetLookAtMatrix() const;

	float4x4 GetViewMatrix() const;
	float4x4 GetProjectionMatrix() const;

	void SetFOV(float newHorizontalFov);
	void SetAspectRatio(float newAspectRatio);
	void SetPlaneDistances(float zNear, float zFar);
	void SetCameraPosition(const float3& newPosition);
	void SetCameraFront(const float3& newFront);
	void SetCameraUp(const float3& newUp);
	void SetCameraTarget(const float3& newTarget);

private:
	Frustum camera;

	float3 target;

	float cameraMoveSpeed = 2.0f;
};

