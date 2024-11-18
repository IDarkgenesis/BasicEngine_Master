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

	//Positive Pitch Rotation
	if (downKeyPressed && (currentPitchAngle + cameraRotationAngle) < maximumPositivePitch)
	{
		currentPitchAngle += cameraRotationAngle;

		float3 right = camera.front.Normalized().Cross(camera.up.Normalized()).Normalized();

		float3x3 rotationDeltaMatrix = float3x3(
			float3(1.f, 0.f,						 0.f),
			float3(0,	cosf(cameraRotationAngle),	-sinf(cameraRotationAngle)),
			float3(0,	sinf(cameraRotationAngle),	cosf(cameraRotationAngle))
		);

		float3 oldFront = camera.front.Normalized();
		camera.front = rotationDeltaMatrix.MulDir(oldFront);

		float3 oldUp = camera.up.Normalized();
		camera.up = rotationDeltaMatrix.MulDir(oldUp);

	}

	// Negative Pitch Rotation
	if (upKeyPressed && (currentPitchAngle - cameraRotationAngle) > maximumNegativePitch)
	{
		currentPitchAngle -= cameraRotationAngle;

		float3x3 rotationDeltaMatrix = float3x3(
			float3(1.f, 0.f, 0.f),
			float3(0, cosf(-cameraRotationAngle), -sinf(-cameraRotationAngle)),
			float3(0, sinf(-cameraRotationAngle), cosf(-cameraRotationAngle))
		);

		float3 oldFront = camera.front.Normalized();
		camera.front = rotationDeltaMatrix.MulDir(oldFront);

		float3 oldUp = camera.up.Normalized();
		camera.up = rotationDeltaMatrix.MulDir(oldUp);

	}

	// Positive Yaw Rotation
	if (rgihtKeyPressed)
	{
		float3x3 rotationDeltaMatrix = float3x3(
			float3(cosf(cameraRotationAngle),	0.f,	sinf(cameraRotationAngle)),
			float3(0.f,							1.f,	0.f),
			float3(-sinf(cameraRotationAngle),	0.f,	cosf(cameraRotationAngle))
		);

		float3 oldFront = camera.front.Normalized();
		camera.front = rotationDeltaMatrix.MulDir(oldFront);

		float3 oldUp = camera.up.Normalized();
		camera.up = rotationDeltaMatrix.MulDir(oldUp);
	}

	// Negative Yaw Rotation
	if (leftKeyPressed)
	{
		float3x3 rotationDeltaMatrix = float3x3(
			float3(cosf(-cameraRotationAngle), 0.f, sinf(-cameraRotationAngle)),
			float3(0.f, 1.f, 0.f),
			float3(-sinf(-cameraRotationAngle), 0.f, cosf(-cameraRotationAngle))
		);

		float3 oldFront = camera.front.Normalized();
		camera.front = rotationDeltaMatrix.MulDir(oldFront);

		float3 oldUp = camera.up.Normalized();
		camera.up = rotationDeltaMatrix.MulDir(oldUp);
	}

	float finalCameraSpeed = shiftKeyPressed ? cameraMoveSpeed * 2.f : cameraMoveSpeed;

	if (wKeyPressed) camera.pos += finalCameraSpeed * camera.front;

	if (sKeyPressed) camera.pos -= finalCameraSpeed * camera.front;
	
	if (aKeyPressed) camera.pos -= finalCameraSpeed * camera.WorldRight();
	
	if (dKeyPressed) camera.pos += finalCameraSpeed * camera.WorldRight();

	if (qKeyPressed) camera.pos += finalCameraSpeed * float3::unitY;

	if (eKeyPressed) camera.pos -= finalCameraSpeed * float3::unitY;

	inputModule = nullptr;
	delete inputModule;
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
