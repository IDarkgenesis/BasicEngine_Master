#pragma once

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/include/Geometry/Frustum.h"

const float PI = 3.14159265359;

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

	const float maximumPositivePitch = 89.f * (PI / 180.f);
	const float maximumNegativePitch = -89.f * (PI / 180.f);
	const float maximumYawAngle = 360 * (PI / 180.f);
	const float minimumYawAngle = -maximumYawAngle;
	const float DEGTORAD = PI / 180.f;

	float cameraMoveSpeed = 0.05f;
	float cameraRotationAngle = PI / 180.f;
	float currentPitchAngle = 0.f;
	float currentYawAngle = 0.f;
};

