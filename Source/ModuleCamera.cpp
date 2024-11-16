#include "ModuleCamera.h"
#include "Application.h"
#include "MathGeoLib.h"
#include "ModuleWindow.h"

ModuleCamera::ModuleCamera()
{
	target = float3::zero;

	camera.type = FrustumType::PerspectiveFrustum;

	camera.pos = float3(0, 1, 5);
	camera.front = -float3::unitZ;
	camera.up = float3::unitY;

	camera.nearPlaneDistance = 0.1f;
	camera.farPlaneDistance = 100.f;
	camera.verticalFov = math::pi / 4.0f;
	camera.horizontalFov = 2.f * atanf(tanf(camera.verticalFov * 0.5f)) * (SCREEN_WIDTH / SCREEN_HEIGHT);
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

float4x4 ModuleCamera::GetLookAtMatrix(const float3& cameraPosition, const float3& targetPosition, const float3& upVector)
{
	float3 forward = float3(targetPosition - cameraPosition).Normalized();
	float3 right = float3(forward.Cross(upVector)).Normalized();
	float3 up = float3(right.Cross(forward)).Normalized();

	
	float4x4 viewMatrix = float4x4(float4(right, 0), float4(up, 0), -float4(forward, 0), float4(float3::zero, 1)) * float4x4(float4(1, 0, 0, -cameraPosition.x), float4(0, 1, 0, -cameraPosition.y), float4(0, 0, 1, -cameraPosition.z), float4(0, 0, 0, 1));
	viewMatrix.Transpose();
	return viewMatrix;
}

float4x4 ModuleCamera::GetLookAtMatrix(const float3& targetPosition) const
{
	float3 forward = float3(targetPosition - camera.pos).Normalized();
	float3 right = float3(forward.Cross(camera.up)).Normalized();
	float3 up = float3(right.Cross(forward)).Normalized();


	float4x4 viewMatrix = float4x4(float4(right, 0), float4(up, 0), -float4(forward, 0), float4(float3::zero, 1)) * float4x4(float4(1, 0, 0, -camera.pos.x), float4(0, 1, 0, -camera.pos.y), float4(0, 0, 1, -camera.pos.z), float4(0, 0, 0, 1));
	viewMatrix.Transpose();
	return viewMatrix;
}

float4x4 ModuleCamera::GetLookAtMatrix() const
{
	float3 forward = float3(target - camera.pos).Normalized();
	float3 right = float3(forward.Cross(camera.up)).Normalized();
	float3 up = float3(right.Cross(forward)).Normalized();


	float4x4 viewMatrix = float4x4(float4(right, 0), float4(up, 0), -float4(forward, 0), float4(float3::zero, 1)) * float4x4(float4(1, 0, 0, -camera.pos.x), float4(0, 1, 0, -camera.pos.y), float4(0, 0, 1, -camera.pos.z), float4(0, 0, 0, 1));
	viewMatrix.Transpose();
	return viewMatrix;
}

float4x4 ModuleCamera::GetViewMatrix() const
{
	return camera.ViewMatrix();
}

float4x4 ModuleCamera::GetProjectionMatrix() const
{
	return camera.ProjectionMatrix();
}

void ModuleCamera::SetFOV(float newHorizontalFov)
{
	int* width = nullptr;
	int* height = nullptr;

	SDL_GetWindowSize(App->GetWindow()->window, width, height);

	if (width == nullptr && height == nullptr) return;

	camera.horizontalFov = newHorizontalFov;

	camera.verticalFov = 2.0f * atanf(tanf(camera.horizontalFov * 0.5f) * (float)(*height/ *width));
}

void ModuleCamera::SetAspectRatio(float newAspectRatio)
{
	camera.verticalFov = 2.0f * atanf(tanf(camera.horizontalFov * 0.5f) * newAspectRatio);
}

void ModuleCamera::SetPlaneDistances(float zNear, float zFar)
{
	camera.nearPlaneDistance = zNear;
	camera.farPlaneDistance = zFar;
}

void ModuleCamera::SetCameraPosition(const float3& newPosition)
{
	camera.pos = newPosition;
}

void ModuleCamera::SetCameraFront(const float3& newFront)
{
	camera.front = newFront;
}

void ModuleCamera::SetCameraUp(const float3& newUp)
{
	camera.up = newUp;
}

void ModuleCamera::SetCameraTarget(const float3& newTarget)
{
	target = newTarget;
}
