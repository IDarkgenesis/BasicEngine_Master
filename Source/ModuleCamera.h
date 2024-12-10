#pragma once

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/include/Geometry/Frustum.h"

const float PI = 3.14159265359f;

enum WASD_Direction
{
	FRONT,
	RIGHT,
	LEFT,
	BACK,
	FRONT_RIGHT,
	FRONT_LEFT,
	BACK_RIGHT,
	BACK_LEFT,
	NONE
};

class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera() override;

	bool Init() override;
	update_status Update(float deltaTime) override;
	bool CleanUp() override;

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

	float3 target = float3::zero;

	const float maximumPositivePitch = 89.f * (PI / 180.f);
	const float maximumNegativePitch = -89.f * (PI / 180.f);
	const float DEGTORAD = PI / 180.f;

	float movementScaleFactor = 1.f;

	float cameraMoveSpeed = 7.5f;
	float cameraRotationAngle = 135.f * DEGTORAD;
	float mouseSensitivity = 0.5f;
	
	float currentPitchAngle = 0.f;

	bool orbiting = false;

	void RotateYaw(float deltaAngle);
	void RotatePitch(float deltaAngle);
	WASD_Direction GetWasdDirection();
};

