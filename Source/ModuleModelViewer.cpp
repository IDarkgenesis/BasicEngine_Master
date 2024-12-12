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
    // Creating program for texutre models
    programTextures = App->GetProgramModule()->LoadShaders("./RenderExercise/VertexShader.glsl", "./RenderExercise/FragmentShader.glsl");

    // LOADING HOUSE MODEL
    LoadModel("../../ModelsTextures/BakerHouse.gltf");
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

void ModuleModelViewer::LoadModel(const char* path)
{
    currentLoadedModel->Load(path);

    float3 maximumValues = currentLoadedModel->GetMaximumValues();

    float3 boundingBoxCenter = maximumValues / 2.f;

    App->GetCameraModule()->SetCameraTarget(boundingBoxCenter);

    App->GetCameraModule()->FocusGeometry();
}

void ModuleModelViewer::LoadTexture(const char* path)
{
    currentLoadedModel->LoadAdditionalTexture(path);
}

int ModuleModelViewer::GetModelLoadedTextures() const
{
    return currentLoadedModel->GetTotalLoadedTextures();
}

int ModuleModelViewer::GetModelRenderTexture() const
{
    return currentLoadedModel->GetRenderTexture();
}

void ModuleModelViewer::SetModelRenderTexture(int renderTexture)
{
    currentLoadedModel->SetRenderTexture(renderTexture);
}
