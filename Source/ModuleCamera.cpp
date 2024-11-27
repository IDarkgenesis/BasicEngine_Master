#include "ModuleCamera.h"
#include "Application.h"
#include "MathGeoLib.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"

ModuleCamera::ModuleCamera()
{
	target = float3::zero;

	camera.type = FrustumType::PerspectiveFrustum;

	camera.pos = float3(0, 1, 5);
	camera.front = -float3::unitZ;
	camera.up = float3::unitY;

	camera.nearPlaneDistance = 0.1f;
	camera.farPlaneDistance = 100.f;

	camera.horizontalFov = 90.f * DEGTORAD;
	camera.verticalFov = 2.0f * atanf(tanf(camera.horizontalFov * 0.5f) * ((float)SCREEN_HEIGHT / (float)SCREEN_WIDTH));;
}

ModuleCamera::~ModuleCamera()
{

}

bool ModuleCamera::Init()
{
	return true;
}

update_status ModuleCamera::Update(float deltaTime)
{
	ModuleInput* inputModule = App->GetInput();
	
	if(inputModule == nullptr) return UPDATE_CONTINUE;

	bool shiftKeyPressed = inputModule->GetKey(SDL_SCANCODE_LSHIFT) == KeyState::KEY_DOWN || inputModule->GetKey(SDL_SCANCODE_LSHIFT) == KeyState::KEY_REPEAT;
	bool wKeyPressed = inputModule->GetKey(SDL_SCANCODE_W) == KeyState::KEY_DOWN || inputModule->GetKey(SDL_SCANCODE_W) == KeyState::KEY_REPEAT;
	bool sKeyPressed = inputModule->GetKey(SDL_SCANCODE_S) == KeyState::KEY_DOWN || inputModule->GetKey(SDL_SCANCODE_S) == KeyState::KEY_REPEAT;
	bool aKeyPressed = inputModule->GetKey(SDL_SCANCODE_A) == KeyState::KEY_DOWN || inputModule->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT;
	bool dKeyPressed = inputModule->GetKey(SDL_SCANCODE_D) == KeyState::KEY_DOWN || inputModule->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT;
	bool qKeyPressed = inputModule->GetKey(SDL_SCANCODE_Q) == KeyState::KEY_DOWN || inputModule->GetKey(SDL_SCANCODE_Q) == KeyState::KEY_REPEAT;
	bool eKeyPressed = inputModule->GetKey(SDL_SCANCODE_E) == KeyState::KEY_DOWN || inputModule->GetKey(SDL_SCANCODE_E) == KeyState::KEY_REPEAT;
	bool rgihtKeyPressed = inputModule->GetKey(SDL_SCANCODE_RIGHT) == KeyState::KEY_DOWN || inputModule->GetKey(SDL_SCANCODE_RIGHT) == KeyState::KEY_REPEAT;
	bool leftKeyPressed = inputModule->GetKey(SDL_SCANCODE_LEFT) == KeyState::KEY_DOWN || inputModule->GetKey(SDL_SCANCODE_LEFT) == KeyState::KEY_REPEAT;
	bool upKeyPressed = inputModule->GetKey(SDL_SCANCODE_UP) == KeyState::KEY_DOWN || inputModule->GetKey(SDL_SCANCODE_UP) == KeyState::KEY_REPEAT;
	bool downKeyPressed = inputModule->GetKey(SDL_SCANCODE_DOWN) == KeyState::KEY_DOWN || inputModule->GetKey(SDL_SCANCODE_DOWN) == KeyState::KEY_REPEAT;
	bool leftClick = inputModule->GetMouseButtonDown(SDL_BUTTON_LEFT);
	bool rightClick = inputModule->GetMouseButtonDown(SDL_BUTTON_RIGHT);
	const float2 mouseMove = inputModule->GetMouseMotion();

	// Down Pitch Rotation
	if (downKeyPressed && (currentPitchAngle + cameraRotationAngle) < maximumPositivePitch)
	{
		currentPitchAngle += cameraRotationAngle;

		RotatePitch(-cameraRotationAngle);
	}

	// Up Pitch Rotation
	if (upKeyPressed && (currentPitchAngle - cameraRotationAngle) > maximumNegativePitch)
	{
		currentPitchAngle -= cameraRotationAngle;

		RotatePitch(cameraRotationAngle);
	}

	// Right Yaw Rotation
	if (rgihtKeyPressed)
	{
		RotateYaw(-cameraRotationAngle);
	}

	// Left Yaw Rotation
	if (leftKeyPressed)
	{
		RotateYaw(cameraRotationAngle);
	}

	if (rightClick) {
		const float2 mouseMotion = inputModule->GetMouseMotion();
		if (mouseMotion.y > 0 && (currentPitchAngle + cameraRotationAngle) < maximumPositivePitch)
		{
			currentPitchAngle += cameraRotationAngle;
			RotatePitch(-cameraRotationAngle);
		}
		else if (mouseMotion.y < 0 && (currentPitchAngle - cameraRotationAngle) > maximumNegativePitch)
		{
			currentPitchAngle -= cameraRotationAngle;
			RotatePitch(cameraRotationAngle);
		}
		if (mouseMotion.x > 0) RotateYaw(-cameraRotationAngle);
		if (mouseMotion.x < 0) RotateYaw(cameraRotationAngle);
	}

	float finalCameraSpeed = shiftKeyPressed ? cameraMoveSpeed * 2.f : cameraMoveSpeed;

	if (wKeyPressed) camera.pos += finalCameraSpeed * camera.front;

	if (sKeyPressed) camera.pos -= finalCameraSpeed * camera.front;
	
	if (aKeyPressed) camera.pos -= finalCameraSpeed * camera.WorldRight();
	
	if (dKeyPressed) camera.pos += finalCameraSpeed * camera.WorldRight();

	if (qKeyPressed) camera.pos += finalCameraSpeed * float3::unitY;

	if (eKeyPressed) camera.pos -= finalCameraSpeed * float3::unitY;

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

	camera.verticalFov = 2.0f * atanf(tanf(camera.horizontalFov * 0.5f) * (float)(*height / *width));
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

void ModuleCamera::RotateYaw(float deltaAngle)
{
	float theta = deltaAngle / 2.f;
	Quat quatRotator = Quat(0 * sinf(theta), 1 * sinf(theta), 0 * sinf(theta), cosf(theta));

	float3 oldFront = camera.front.Normalized();
	camera.front = quatRotator.Mul(oldFront);

	float3 oldUp = camera.up.Normalized();
	camera.up = quatRotator.Mul(oldUp);
}

void ModuleCamera::RotatePitch(float deltaAngle)
{
	float3 normFront = camera.front.Normalized();
	float3 normUp = camera.up.Normalized();

	float3 right = normFront.Cross(normUp).Normalized();
	float theta = deltaAngle / 2.f;

	Quat quatRotator = Quat(right.x * sinf(theta), right.y * sinf(theta), right.z * sinf(theta), cosf(theta));

	float3 oldFront = camera.front.Normalized();
	camera.front = quatRotator.Mul(oldFront);

	float3 oldUp = camera.up.Normalized();
	camera.up = quatRotator.Mul(oldUp);
}
