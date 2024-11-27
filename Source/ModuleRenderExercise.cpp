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

	program = App->GetProgram()->CreateProgram(vertexId, fragmentId);

	free(vertexShader);
	free(fragmentShader);

	// Texture loading
	auto baboonPath = L"baboon.ppm";
	DirectX::TexMetadata metadata;
	DirectX::ScratchImage image;
	OpenGLMetadata openGlMeta;

	App->GetTexture()->LoadTexture(baboonPath, metadata, image);
	ModuleTexture::ConvertMetadata(metadata, openGlMeta);

	glGenTextures(1, &baboonTexture);
	glBindTexture(GL_TEXTURE_2D, baboonTexture);
	
	// Sending texture to OpgenGL
	glTexImage2D(GL_TEXTURE_2D, 0, openGlMeta.internalFormat, metadata.width, metadata.height, 0, openGlMeta.format, openGlMeta.type, image.GetPixels());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// Creating VBO for triangle
	
	float vtx_data[] = { 
		-1.0f, -1.0f, 0.0f, 
		1.0f, -1.0f, 0.0f, 
		0.0f, 1.0f, 0.0f,
		
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.5f, 0.0f
	};
	
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
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
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * 3));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, baboonTexture);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp()
{
	if (program > 0) glDeleteProgram(program);
	
	glDeleteBuffers(1, &vbo);

	glDeleteTextures(1, &baboonTexture);

	return true;
}

