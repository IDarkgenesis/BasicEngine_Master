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
    // Creating shaders and program for no texutre models
    char* vertexShader = App->GetProgramModule()->LoadShaderSource("./RenderMeshes/vertexMeshes.glsl");
    char* fragmentShader = App->GetProgramModule()->LoadShaderSource("./RenderMeshes/fragmentMeshes.glsl");

    unsigned vertexId = App->GetProgramModule()->CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned fragmentId = App->GetProgramModule()->CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    programNoTextures = App->GetProgramModule()->CreateProgram(vertexId, fragmentId);

    free(vertexShader);
    free(fragmentShader);

    vertexShader = App->GetProgramModule()->LoadShaderSource("./RenderExercise/VertexShader.glsl");
    fragmentShader = App->GetProgramModule()->LoadShaderSource("./RenderExercise/FragmentShader.glsl");

    vertexId = App->GetProgramModule()->CompileShader(GL_VERTEX_SHADER, vertexShader);
    fragmentId = App->GetProgramModule()->CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // Creating shaders and program for textured models
    programTextures = App->GetProgramModule()->CreateProgram(vertexId, fragmentId);

    free(vertexShader);
    free(fragmentShader);

    // LOADING HOUSE MODEL
    house->Load("../../ModelsTextures/BakerHouse.gltf");

    //float4x4 houseModelMatrix = float4x4::FromTRS(
    //    float3(0.0f, 0.0f, 0.0f),
    //    float4x4::identity,
    //    float3(100.0f, 100.0f, 100.0f)
    //);

    //house->SetModelMatrix(houseModelMatrix);

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
