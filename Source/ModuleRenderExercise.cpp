#include "Globals.h"
#include "Application.h"
#include "ModuleRenderExercise.h"
#include "glew-2.1.0/include/GL/glew.h"
#include "ModuleProgram.h"
#include "MathGeoLib.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"
#include "DirectXTex/DirectXTex.h"

ModuleRenderExercise::ModuleRenderExercise()
{
}

ModuleRenderExercise::~ModuleRenderExercise()
{
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

	// Texture loading
	auto baboonPath = L"baboon.ppm";
	DirectX::TexMetadata metadata;
	DirectX::ScratchImage image;

	bool imageLoaded = App->GetTexture()->LoadTexture(baboonPath, metadata, image);

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
	float4x4 model, view, proj;

	proj = App->GetCamera()->GetProjectionMatrix();
	view = App->GetCamera()->GetViewMatrix();
	
	model = float4x4::identity;
	//model = float4x4::FromTRS(
	//			float3(2.0f, 0.0f, 0.0f),
	//			float4x4::RotateZ(pi / 4.0f),
	//			float3(2.0f, 1.0f, 1.0f));

	glUseProgram(Program);
	glUniformMatrix4fv(0, 1, GL_TRUE, &proj[0][0]);
	glUniformMatrix4fv(1, 1, GL_TRUE, &view[0][0]);
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

