#include "ModuleCamera.h"
#include "Application.h"
#include "MathGeoLib.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleModelViewer.h"

ModuleCamera::ModuleCamera()
{
	
}

ModuleCamera::~ModuleCamera()
{

}

bool ModuleCamera::Init()
{
	target = float3::zero;

	camera.type = FrustumType::PerspectiveFrustum;

	camera.pos = float3(0, 1, 5);
	camera.front = -float3::unitZ;
	camera.up = float3::unitY;

	camera.nearPlaneDistance = 0.1f;
	camera.farPlaneDistance = 100.f;

	camera.horizontalFov = (float)HFOV * DEGTORAD;

	int width = 0;
	int height = 0;
	SDL_GetWindowSize(App->GetWindowModule()->window, &width, &height);

	camera.verticalFov = 2.0f * atanf(tanf(camera.horizontalFov * 0.5f) * ((float)height / (float)width));

	return true;
}

update_status ModuleCamera::Update(float deltaTime)
{
	ModuleInput* inputModule = App->GetInputModule();



	if (inputModule == nullptr) return UPDATE_CONTINUE;

	// Changing camera speed based on SHIFT key press
	bool shiftKeyPressed = inputModule->GetKey(SDL_SCANCODE_LSHIFT) == KeyState::KEY_DOWN || inputModule->GetKey(SDL_SCANCODE_LSHIFT) == KeyState::KEY_REPEAT;
	float finalCameraSpeed = shiftKeyPressed ? cameraMoveSpeed * deltaTime * 2.f * movementScaleFactor : cameraMoveSpeed * deltaTime * movementScaleFactor;

	// Focus the scene object
	bool fKeyPressed = inputModule->GetKey(SDL_SCANCODE_F) == KeyState::KEY_DOWN || inputModule->GetKey(SDL_SCANCODE_F) == KeyState::KEY_REPEAT;
	if (fKeyPressed) FocusGeometry();

	// Orbiting camera
	bool leftClick = inputModule->GetMouseButtonDown(SDL_BUTTON_LEFT);
	bool lAltKeyPressed = inputModule->GetKey(SDL_SCANCODE_LALT) == KeyState::KEY_DOWN || inputModule->GetKey(SDL_SCANCODE_LALT) == KeyState::KEY_REPEAT;
	if (leftClick && lAltKeyPressed)
	{
		const float2 mouseMotion = inputModule->GetMouseMotion();
		float angleX, angleY;

		angleY = mouseMotion.y * DEGTORAD * mouseSensitivity;
		angleX = mouseMotion.x * DEGTORAD * mouseSensitivity;

		float3 maxModelValues = App->GetModelViewerModule()->GetModelMaximumValues();
		float3 minModelValues = App->GetModelViewerModule()->GetModelMinimumValues();

		float3 yawRotateVector = (float3(target.x, maxModelValues.y, target.z) - target).Normalized();

		if (angleX)
		{
			RotateOrbitYaw(-angleX, yawRotateVector);
		}

		if (angleY > 0 && (currentPitchAngle - angleY) > maximumNegativePitch)
		{
			currentPitchAngle -= angleY;
			RotateOrbitPitch(-angleY);
		}
		else if (angleY < 0 && (currentPitchAngle - angleY) < maximumPositivePitch)
		{
			currentPitchAngle -= angleY;
			RotateOrbitPitch(-angleY);
		}
	}

	// Allow camera rotation with arrow keys
	bool rgihtKeyPressed = inputModule->GetKey(SDL_SCANCODE_RIGHT) == KeyState::KEY_DOWN || inputModule->GetKey(SDL_SCANCODE_RIGHT) == KeyState::KEY_REPEAT;
	bool leftKeyPressed = inputModule->GetKey(SDL_SCANCODE_LEFT) == KeyState::KEY_DOWN || inputModule->GetKey(SDL_SCANCODE_LEFT) == KeyState::KEY_REPEAT;
	bool upKeyPressed = inputModule->GetKey(SDL_SCANCODE_UP) == KeyState::KEY_DOWN || inputModule->GetKey(SDL_SCANCODE_UP) == KeyState::KEY_REPEAT;
	bool downKeyPressed = inputModule->GetKey(SDL_SCANCODE_DOWN) == KeyState::KEY_DOWN || inputModule->GetKey(SDL_SCANCODE_DOWN) == KeyState::KEY_REPEAT;

	float deltaRotationAngle = cameraRotationAngle * deltaTime;
	// Down Pitch Rotation
	if (downKeyPressed && (currentPitchAngle + deltaRotationAngle) < maximumPositivePitch)
	{
		currentPitchAngle += deltaRotationAngle;

		RotatePitch(-deltaRotationAngle);
	}

	// Up Pitch Rotation
	if (upKeyPressed && (currentPitchAngle - deltaRotationAngle) > maximumNegativePitch)
	{
		currentPitchAngle -= deltaRotationAngle;

		RotatePitch(deltaRotationAngle);
	}

	// Right Yaw Rotation
	if (rgihtKeyPressed)
	{
		RotateYaw(-deltaRotationAngle);
	}

	// Left Yaw Rotation
	if (leftKeyPressed)
	{
		RotateYaw(deltaRotationAngle);
	}

	
	

	
	// Allow camera movement and rotation if mouse right click
	bool rightClick = inputModule->GetMouseButtonDown(SDL_BUTTON_RIGHT);

	if (rightClick) {

		const float2 mouseMotion = inputModule->GetMouseMotion();
		float angleX, angleY;

		angleY = mouseMotion.y * DEGTORAD * mouseSensitivity;
		angleX = mouseMotion.x * DEGTORAD * mouseSensitivity;

		if (angleY > 0 && (currentPitchAngle - angleY) > maximumNegativePitch)
		{
			currentPitchAngle -= angleY;
			RotatePitch(-angleY);
		}
		else if (angleY < 0 && (currentPitchAngle - angleY) < maximumPositivePitch)
		{
			currentPitchAngle -= angleY;
			RotatePitch(-angleY);
		}

		if (angleX)
		{
			RotateYaw(-angleX);
		}

		// WASD Camera movement

		WASD_Direction cameraWasd = GetWasdDirection();

		switch (cameraWasd)
		{
		case FRONT:
			camera.pos += finalCameraSpeed * camera.front;
			break;
		case RIGHT:
			camera.pos += finalCameraSpeed * camera.WorldRight();
			break;
		case LEFT:
			camera.pos -= finalCameraSpeed * camera.WorldRight();
			break;
		case BACK:
			camera.pos -= finalCameraSpeed * camera.front;
			break;
		case FRONT_RIGHT:
			camera.pos += (finalCameraSpeed / 2.f) * camera.front;
			camera.pos += (finalCameraSpeed / 2.f) * camera.WorldRight();
			break;
		case FRONT_LEFT:
			camera.pos += (finalCameraSpeed / 2.f) * camera.front;
			camera.pos -= (finalCameraSpeed / 2.f) * camera.WorldRight();
			break;
		case BACK_RIGHT:
			camera.pos -= (finalCameraSpeed / 2.f) * camera.front;
			camera.pos += (finalCameraSpeed / 2.f) * camera.WorldRight();
			break;
		case BACK_LEFT:
			camera.pos -= (finalCameraSpeed / 2.f) * camera.front;
			camera.pos -= (finalCameraSpeed / 2.f) * camera.WorldRight();
			break;
		default:
			break;
		}

		// Adjusting camera height
		bool qKeyPressed = inputModule->GetKey(SDL_SCANCODE_Q) == KeyState::KEY_DOWN || inputModule->GetKey(SDL_SCANCODE_Q) == KeyState::KEY_REPEAT;
		bool eKeyPressed = inputModule->GetKey(SDL_SCANCODE_E) == KeyState::KEY_DOWN || inputModule->GetKey(SDL_SCANCODE_E) == KeyState::KEY_REPEAT;
		
		if (qKeyPressed) camera.pos += finalCameraSpeed * float3::unitY;
		if (eKeyPressed) camera.pos -= finalCameraSpeed * float3::unitY;

		// Zoom in out
		int mouseWheel = inputModule->GetMouseWheel();
		if (mouseWheel)
		{
			camera.pos += (mouseWheel * zoomSensitivity) * finalCameraSpeed * camera.front;
		}
	}
	


	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
	return false;
}

void ModuleCamera::FocusGeometry()
{
	float3 maxModelValues = App->GetModelViewerModule()->GetModelMaximumValues();
	float3 minModelValues = App->GetModelViewerModule()->GetModelMinimumValues();

	float zPosition = maxModelValues.z ? maxModelValues.z * 5.f : 5.f;

	camera.pos = float3(maxModelValues.x / 2.f, maxModelValues.y / 2.f, zPosition);
	camera.front = -float3::unitZ;
	camera.up = float3::unitY;
	
	float nearDisctance = abs(camera.pos.z / 10.f);
	camera.nearPlaneDistance = nearDisctance? nearDisctance : 0.1f;
	
	movementScaleFactor = zPosition / 7.f;

	currentPitchAngle = 0;
}

float4x4 ModuleCamera::GetLookAtMatrix(const float3& cameraPosition, const float3& targetPosition, const float3& upVector)
{
	float3 forward = float3(targetPosition - cameraPosition).Normalized();
	float3 right = float3(forward.Cross(upVector)).Normalized();
	float3 up = float3(right.Cross(forward)).Normalized();

	
	float4x4 viewMatrix = float4x4(
		float4(right, -cameraPosition.Dot(right)),
		float4(up, -cameraPosition.Dot(up)),
		float4(-forward, cameraPosition.Dot(forward)),
		float4(0.f, 0.f, 0.f, 1)
	);

	viewMatrix.Transpose();
	return viewMatrix;
}

float4x4 ModuleCamera::GetLookAtMatrix(const float3& targetPosition) const
{
	float3 forward = float3(targetPosition - camera.pos).Normalized();
	float3 right = float3(forward.Cross(camera.up)).Normalized();
	float3 up = float3(right.Cross(forward)).Normalized();


	float4x4 viewMatrix = float4x4(
		float4(right, -camera.pos.Dot(right)),
		float4(up, -camera.pos.Dot(up)),
		float4(-forward, camera.pos.Dot(forward)),
		float4(0.f, 0.f, 0.f, 1)
	);

	viewMatrix.Transpose();
	return viewMatrix;
}

float4x4 ModuleCamera::GetLookAtMatrix() const
{
	float3 forward = float3(target - camera.pos).Normalized();
	float3 right = float3(forward.Cross(camera.up)).Normalized();
	float3 up = float3(right.Cross(forward)).Normalized();


	float4x4 viewMatrix = float4x4(
		float4(right, -camera.pos.Dot(right)), 
		float4(up, -camera.pos.Dot(up)), 
		float4(-forward, camera.pos.Dot(forward)),
		float4(0.f,0.f,0.f, 1)
		);

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
	int width = 0;
	int height = 0;

	SDL_GetWindowSize(App->GetWindowModule()->window, &width, &height);

	if (!width && !height) return;

	camera.horizontalFov = newHorizontalFov * DEGTORAD;

	camera.verticalFov = 2.0f * atanf(tanf(camera.horizontalFov * 0.5f) * ((float)height / (float)width));
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

void ModuleCamera::RotateOrbitYaw(float deltaAngle, const float3& rotateFromVector)
{
	float3 rotateNormalized = rotateFromVector.Normalized();

	float theta = deltaAngle / 2.f;
	Quat quatRotator = Quat(rotateNormalized.x * sinf(theta), rotateNormalized.y * sinf(theta), rotateNormalized.z * sinf(theta), cosf(theta));

	float3 oldCameraPos = camera.pos;
	camera.pos = quatRotator.Mul(oldCameraPos);

	float3 oldUp = camera.up;
	camera.up = quatRotator.Mul(oldUp);

	float3 oldFront = camera.front;
	camera.front = quatRotator.Mul(oldFront);
}

void ModuleCamera::RotateOrbitPitch(float deltaAngle)
{
	float3 normFront = camera.front.Normalized();
	float3 normUp = camera.up.Normalized();

	float3 right = normFront.Cross(normUp).Normalized();
	float theta = deltaAngle / 2.f;

	Quat quatRotator = Quat(right.x * sinf(theta), right.y * sinf(theta), right.z * sinf(theta), cosf(theta));

	float3 oldCameraPos = camera.pos;
	camera.pos = quatRotator.Mul(oldCameraPos);

	float3 oldFront = camera.front.Normalized();
	camera.front = quatRotator.Mul(oldFront);

	float3 oldUp = camera.up.Normalized();
	camera.up = quatRotator.Mul(oldUp);
}

WASD_Direction ModuleCamera::GetWasdDirection()
{
	ModuleInput* inputModule = App->GetInputModule();

	if (inputModule == nullptr) return NONE;

	bool forward = inputModule->GetKey(SDL_SCANCODE_W) == KeyState::KEY_DOWN || inputModule->GetKey(SDL_SCANCODE_W) == KeyState::KEY_REPEAT;
	bool right = inputModule->GetKey(SDL_SCANCODE_D) == KeyState::KEY_DOWN || inputModule->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT;
	bool left = inputModule->GetKey(SDL_SCANCODE_A) == KeyState::KEY_DOWN || inputModule->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT;
	bool back = inputModule->GetKey(SDL_SCANCODE_S) == KeyState::KEY_DOWN || inputModule->GetKey(SDL_SCANCODE_S) == KeyState::KEY_REPEAT;

	if ( (forward && !right && !left && !back) || (forward && left && right && !back) ) return FRONT;
	if ( (!forward && right && !left && !back) || (forward && !left && right && back) ) return RIGHT;
	if ( (!forward && !right && left && !back) || (forward && left && !right && back) ) return LEFT;
	if ( (!forward && !right && !left && back) || (!forward && left && right && back) ) return BACK;
	if (forward && right && !left && !back) return FRONT_RIGHT;
	if (forward && !right && left && !back) return FRONT_LEFT;
	if (!forward && right && !left && back) return BACK_RIGHT;
	if (!forward && !right && left && back) return BACK_LEFT;

	return NONE;
}
