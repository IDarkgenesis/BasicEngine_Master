#include "Globals.h"
#include "Application.h"
#include "ModuleRenderExercise.h"
#include "glew-2.1.0/include/GL/glew.h"
#include "ModuleProgram.h"
#include "MathGeoLib.h"

ModuleRenderExercise::ModuleRenderExercise()
{
}

ModuleRenderExercise::~ModuleRenderExercise()
{
}

float4x4 GetLookAtMatrix(float3 cameraPosition, float3 targetPosition, float3 upVector)
{

	float3 forward = float3(targetPosition - cameraPosition).Normalized();
	float3 right = float3(forward.Cross(upVector)).Normalized();
	float3 up = float3(right.Cross(forward)).Normalized();

	//return float4x4(float4(right, 0), float4(up, 0), -float4(forward, 0), float4(float3::zero, 1)) * float4x4(float4(1,0,0,-cameraPosition.x), float4(0,1,0, -cameraPosition.y), float4(0,0,1, -cameraPosition.z), float4(0,0,0,1));
	float4x4 viewMatrix = float4x4(float4(right, 0), float4(up, 0), -float4(forward, 0), float4(float3::zero, 1)) * float4x4(float4(1, 0, 0, -cameraPosition.x), float4(0, 1, 0, -cameraPosition.y), float4(0, 0, 1, -cameraPosition.z), float4(0, 0, 0, 1));
	viewMatrix.Transpose();
	return viewMatrix;
}

bool ModuleRenderExercise::Init()
{
	// Creating shaders and program
	char* vertexShader = App->GetProgram()->LoadShaderSource("VertexShader.glsl");
	char* fragmentShader = App->GetProgram()->LoadShaderSource("FragmentShader.glsl");

	unsigned vertexId = App->GetProgram()->CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned fragmentId = App->GetProgram()->CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	Program = App->GetProgram()->CreateProgram(vertexId, fragmentId);

	free(vertexShader);
	free(fragmentShader);

	// Creating VBO for triangle
	float vtx_data[] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
	
	glGenBuffers(1, &Vbo);
	glBindBuffer(GL_ARRAY_BUFFER, Vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);

	return true;
}

update_status ModuleRenderExercise::Update()
{
	
	// Creating matrices for rendering
	float4x4 model, view, proj, view2;

	Frustum frustum;
	frustum.type = FrustumType::PerspectiveFrustum;

	frustum.pos = float3(0,1,5);
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;

	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f)) * frustum.AspectRatio();

	

	proj = frustum.ProjectionMatrix();

	view = frustum.ViewMatrix();
	view2 = GetLookAtMatrix(frustum.pos, float3(0,-1,0), float3::unitY);
	
	model = float4x4::identity;

	glUseProgram(Program);
	glUniformMatrix4fv(0, 1, GL_TRUE, &proj[0][0]);
	glUniformMatrix4fv(1, 1, GL_TRUE, &view2[0][0]);
	glUniformMatrix4fv(2, 1, GL_TRUE, &model[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, Vbo);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp()
{
	if (Program > 0) glDeleteProgram(Program);
	glDeleteBuffers(1, &Vbo);

	return true;
}

