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
	program = App->GetProgramModule()->LoadShaders("./RenderExercise/VertexShader.glsl", "./RenderExercise/FragmentShader.glsl");

	// Texture loading
	DirectX::TexMetadata textureMetadata;
	auto baboonPath = L"./RenderExercise/baboon.ppm";
	baboonTexture = App->GetTextureModule()->LoadTexture(baboonPath, textureMetadata);
	// Creating VBO for triangle
	
	//float vtx_data[] = { 
	//	-1.0f, -1.0f, 0.0f, 
	//	1.0f, -1.0f, 0.0f, 
	//	0.0f, 1.0f, 0.0f,
	//	
	//	0.0f, 1.0f,
	//	1.0f, 1.0f,
	//	0.5f, 0.0f
	//};

	float vtx_data[] = {
		-1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,

		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,

		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
	};
	
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);

	return true;
}

update_status ModuleRenderExercise::Update(float deltaTime)
{
	
	// Creating matrices for rendering
	float4x4 model, view, proj;

	proj = App->GetCameraModule()->GetProjectionMatrix();
	view = App->GetCameraModule()->GetViewMatrix();
	
	model = float4x4::identity;
	//model = float4x4::FromTRS(
	//			float3(2.0f, 0.0f, 0.0f),
	//			float4x4::RotateZ(pi / 4.0f),
	//			float3(2.0f, 1.0f, 1.0f));

	glUseProgram(program);
	glUniformMatrix4fv(0, 1, GL_TRUE, &proj[0][0]);
	glUniformMatrix4fv(1, 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(2, 1, GL_TRUE, &model[0][0]);

	// Sending triangle vertices
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Sending texture coordiantes
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * 6));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, baboonTexture);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp()
{
	if (program > 0) glDeleteProgram(program);
	
	glDeleteBuffers(1, &vbo);

	glDeleteTextures(1, &baboonTexture);

	return true;
}

