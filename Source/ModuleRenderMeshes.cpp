#include "ModuleRenderMeshes.h"
#include "Application.h"
#include "EngineModel.h"
#include "glew-2.1.0/include/GL/glew.h"
#include "ModuleProgram.h"
#include "MathGeoLib.h"
#include "ModuleCamera.h"

ModuleRenderMeshes::ModuleRenderMeshes()
{
    house = new EngineModel();
}

ModuleRenderMeshes::~ModuleRenderMeshes()
{
    delete house;
}

bool ModuleRenderMeshes::Init()
{
    // Creating program for no texutre models
    programNoTextures = App->GetProgramModule()->LoadShaders("./RenderMeshes/vertexMeshes.glsl", "./RenderMeshes/fragmentMeshes.glsl");

    // Creating program for textured models
    programTextures = App->GetProgramModule()->LoadShaders("./RenderExercise/VertexShader.glsl", "./RenderExercise/FragmentShader.glsl");

    // LOADING HOUSE MODEL
    house->Load("../../ModelsTextures/BakerHouse.gltf");

    return true;
}

update_status ModuleRenderMeshes::Update(float deltaTime)
{

    float4x4 proj, view;

    proj = App->GetCameraModule()->GetProjectionMatrix();
    view = App->GetCameraModule()->GetViewMatrix();

    house->Render(programTextures, proj, view);
    return UPDATE_CONTINUE;
}

bool ModuleRenderMeshes::CleanUp()
{
    if (programNoTextures > 0) glDeleteProgram(programNoTextures);
    if (programTextures > 0) glDeleteProgram(programTextures);

    return true;
}
