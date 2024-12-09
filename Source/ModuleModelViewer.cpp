#include "ModuleModelViewer.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include "EngineModel.h"
#include "glew-2.1.0/include/GL/glew.h"

ModuleModelViewer::ModuleModelViewer()
{
	currentLoadedModel = new EngineModel();
}

ModuleModelViewer::~ModuleModelViewer()
{
	delete currentLoadedModel;
}

bool ModuleModelViewer::Init()
{
    // Creating shaders and program for texutre models
    char* vertexShader = App->GetProgramModule()->LoadShaderSource("./RenderExercise/VertexShader.glsl");
    char* fragmentShader = App->GetProgramModule()->LoadShaderSource("./RenderExercise/FragmentShader.glsl");

    unsigned vertexId = App->GetProgramModule()->CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned fragmentId = App->GetProgramModule()->CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    vertexId = App->GetProgramModule()->CompileShader(GL_VERTEX_SHADER, vertexShader);
    fragmentId = App->GetProgramModule()->CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    programTextures = App->GetProgramModule()->CreateProgram(vertexId, fragmentId);

    free(vertexShader);
    free(fragmentShader);

    // LOADING HOUSE MODEL
    currentLoadedModel->Load("../../ModelsTextures/BakerHouse.gltf");
	
    return true;
}

update_status ModuleModelViewer::Update(float deltaTime)
{
    float4x4 proj, view;

    proj = App->GetCameraModule()->GetProjectionMatrix();
    view = App->GetCameraModule()->GetViewMatrix();

    currentLoadedModel->Render(programTextures, proj, view);

	return UPDATE_CONTINUE;
}

bool ModuleModelViewer::CleanUp()
{
    if (programTextures > 0) glDeleteProgram(programTextures);

	return false;
}

float3 ModuleModelViewer::GetModelMaximumValues() const
{
    
    return currentLoadedModel->GetMaximumValues();
}

float3 ModuleModelViewer::GetModelMinimumValues() const
{
    return currentLoadedModel->GetMinimumValues();
}
