#include "ModuleRenderMeshes.h"
#include "Application.h"
#include "EngineModel.h"
#include "glew-2.1.0/include/GL/glew.h"
#include "ModuleProgram.h"
#include "MathGeoLib.h"
#include "ModuleCamera.h"

ModuleRenderMeshes::ModuleRenderMeshes()
{
    triangleNoIndices = new EngineModel();
    triangleIndices = new EngineModel();
}

ModuleRenderMeshes::~ModuleRenderMeshes()
{
    delete triangleNoIndices;
    delete triangleIndices;
}

bool ModuleRenderMeshes::Init()
{
    // Creating shaders and program
    char* vertexShader = App->GetProgram()->LoadShaderSource("./RenderMeshes/vertexMeshes.glsl");
    char* fragmentShader = App->GetProgram()->LoadShaderSource("./RenderMeshes/fragmentMeshes.glsl");

    unsigned vertexId = App->GetProgram()->CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned fragmentId = App->GetProgram()->CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    program = App->GetProgram()->CreateProgram(vertexId, fragmentId);

    free(vertexShader);
    free(fragmentShader);

    triangleNoIndices->Load("./RenderMeshes/TriangleWithoutIndices.gltf");
    triangleIndices->Load("./RenderMeshes/Triangle.gltf");

    return true;
}

update_status ModuleRenderMeshes::Update(float deltaTime)
{
    //triangleNoIndices->Render(program);
    triangleIndices->Render(program);

    return UPDATE_CONTINUE;
}

bool ModuleRenderMeshes::CleanUp()
{
    if (program > 0) glDeleteProgram(program);

    return true;
}
