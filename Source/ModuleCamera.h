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

	void FocusGeometry();

	static float4x4 GetLookAtMatrix(const float3& cameraPosition, const float3& targetPosition, const float3& upVector);
	float4x4 GetLookAtMatrix(const float3& targetPosition) const;
	float4x4 GetLookAtMatrix() const;

	float4x4 GetViewMatrix() const;
	float4x4 GetProjectionMatrix() const;

	float GetMoveMentScaleFactor() const { return movementScaleFactor; };
	float GetCameraMoveSpeed() const { return cameraMoveSpeed; };
	float GetMouseSensitivity() const { return mouseSensitivity; };
	float GetZoomSensitivity() const { return zoomSensitivity; };

	void SetFOV(float newHorizontalFov);
	void SetAspectRatio(float newAspectRatio);
	void SetPlaneDistances(float zNear, float zFar);
	void SetCameraPosition(const float3& newPosition);
	void SetCameraFront(const float3& newFront);
	void SetCameraUp(const float3& newUp);
	void SetCameraTarget(const float3& newTarget);

	void SetMoveMentScaleFactor(float newMovementSacle);
	void SetCameraMoveSpeed(float newCameraMoveSpeed);
	void SetMouseSensitivity(float newMouseSensitivity);
	void SetZoomSensitivity(float newZoomSensitivity);

private:
	Frustum camera;

	float3 target = float3::zero;

	const float DEGTORAD = PI / 180.f;
	const float maximumPositivePitch = 89.f * DEGTORAD;
	const float maximumNegativePitch = -89.f * DEGTORAD;
	float cameraRotationAngle = 135.f * DEGTORAD;


	float movementScaleFactor = DEFAULT_CAMERA_MOVEMENT_SCALE_FACTOR;
	float cameraMoveSpeed = DEFAULT_CAMERA_MOVMENT_SPEED;
	float mouseSensitivity = DEFAULT_CAMERA_MOUSE_SENSITIVITY;
	float zoomSensitivity = DEFAUL_CAMERA_ZOOM_SENSITIVITY;
	
	float currentPitchAngle = 0.f;

	void RotateYaw(float deltaAngle);
	void RotatePitch(float deltaAngle);
	void RotateOrbitYaw(float deltaAngle, const float3& rotateFromVector);
	void RotateOrbitPitch(float deltaAngle);
	WASD_Direction GetWasdDirection();
};

