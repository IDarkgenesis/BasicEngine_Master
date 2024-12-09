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
    char* vertexShader = App->GetProgram()->LoadShaderSource("./RenderMeshes/vertexMeshes.glsl");
    char* fragmentShader = App->GetProgram()->LoadShaderSource("./RenderMeshes/fragmentMeshes.glsl");

    unsigned vertexId = App->GetProgram()->CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned fragmentId = App->GetProgram()->CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    programNoTextures = App->GetProgram()->CreateProgram(vertexId, fragmentId);

    free(vertexShader);
    free(fragmentShader);

    vertexShader = App->GetProgram()->LoadShaderSource("./RenderExercise/VertexShader.glsl");
    fragmentShader = App->GetProgram()->LoadShaderSource("./RenderExercise/FragmentShader.glsl");

    vertexId = App->GetProgram()->CompileShader(GL_VERTEX_SHADER, vertexShader);
    fragmentId = App->GetProgram()->CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // Creating shaders and program for textured models
    programTextures = App->GetProgram()->CreateProgram(vertexId, fragmentId);

    free(vertexShader);
    free(fragmentShader);

    // LOADING HOUSE MODEL
    house->Load("../../ModelsTextures/BakerHouse.gltf");

    float4x4 houseModelMatrix = float4x4::FromTRS(
        float3(0.0f, 0.0f, 0.0f),
        float4x4::identity,
        float3(100.0f, 100.0f, 100.0f)
    );

    house->SetModelMatrix(houseModelMatrix);

    return true;
}

update_status ModuleRenderMeshes::Update(float deltaTime)
{

    float4x4 proj, view;

    proj = App->GetCamera()->GetProjectionMatrix();
    view = App->GetCamera()->GetViewMatrix();

    house->Render(programTextures, proj, view);
    return UPDATE_CONTINUE;
}

bool ModuleRenderMeshes::CleanUp()
{
    if (programNoTextures > 0) glDeleteProgram(programNoTextures);
    if (programTextures > 0) glDeleteProgram(programTextures);

    return true;
}
