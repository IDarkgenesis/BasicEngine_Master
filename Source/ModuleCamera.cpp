#include "ModuleCamera.h"
#include "Application.h"
#include "MathGeoLib.h"

ModuleCamera::ModuleCamera()
{
	frustumType = FrustumType::PerspectiveFrustum;
	cameraPosition = float3(0, 1, 5);
	front = -float3::unitZ;
	up = float3::unitY;
	target = float3::zero;

	verticalFov = math::pi / 4.0f;
	horizontalFov = 2.f * atanf(tanf(verticalFov * 0.5f)) * (SCREEN_WIDTH/SCREEN_HEIGHT);

	camera.type = frustumType;

	camera.pos = cameraPosition;
	camera.front = front;
	camera.up = up;

	camera.nearPlaneDistance = 0.1f;
	camera.farPlaneDistance = 100.f;
	camera.horizontalFov = verticalFov;
	camera.verticalFov = horizontalFov;
}

ModuleCamera::~ModuleCamera()
{

}

bool ModuleCamera::Init()
{
	return true;
}

update_status ModuleCamera::Update()
{
	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
	return false;
}

float4x4 ModuleCamera::GetLookAtMatrix(float3 cameraPosition, float3 targetPosition, float3 upVector)
{
	float3 forward = float3(targetPosition - cameraPosition).Normalized();
	float3 right = float3(forward.Cross(upVector)).Normalized();
	float3 up = float3(right.Cross(forward)).Normalized();

	
	float4x4 viewMatrix = float4x4(float4(right, 0), float4(up, 0), -float4(forward, 0), float4(float3::zero, 1)) * float4x4(float4(1, 0, 0, -cameraPosition.x), float4(0, 1, 0, -cameraPosition.y), float4(0, 0, 1, -cameraPosition.z), float4(0, 0, 0, 1));
	viewMatrix.Transpose();
	return viewMatrix;
}

float4x4 ModuleCamera::GetLookAtMatrix(float3 targetPosition)
{
	float3 forward = float3(targetPosition - cameraPosition).Normalized();
	float3 right = float3(forward.Cross(up)).Normalized();
	float3 up = float3(right.Cross(forward)).Normalized();


	float4x4 viewMatrix = float4x4(float4(right, 0), float4(up, 0), -float4(forward, 0), float4(float3::zero, 1)) * float4x4(float4(1, 0, 0, -cameraPosition.x), float4(0, 1, 0, -cameraPosition.y), float4(0, 0, 1, -cameraPosition.z), float4(0, 0, 0, 1));
	viewMatrix.Transpose();
	return viewMatrix;
}

float4x4 ModuleCamera::GetLookAtMatrix()
{
	float3 forward = float3(target - cameraPosition).Normalized();
	float3 right = float3(forward.Cross(up)).Normalized();
	float3 up = float3(right.Cross(forward)).Normalized();


	float4x4 viewMatrix = float4x4(float4(right, 0), float4(up, 0), -float4(forward, 0), float4(float3::zero, 1)) * float4x4(float4(1, 0, 0, -cameraPosition.x), float4(0, 1, 0, -cameraPosition.y), float4(0, 0, 1, -cameraPosition.z), float4(0, 0, 0, 1));
	viewMatrix.Transpose();
	return viewMatrix;
}

float4x4 ModuleCamera::GetViewMatrix()
{
	return camera.ViewMatrix();
}

float4x4 ModuleCamera::GetProjectionMatrix()
{
	return camera.ProjectionMatrix();
}
