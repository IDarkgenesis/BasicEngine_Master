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
    box = new EngineModel();
    textureBox = new EngineModel();
    house = new EngineModel();
}

ModuleRenderMeshes::~ModuleRenderMeshes()
{
    delete triangleNoIndices;
    delete triangleIndices;
    delete box;
    delete textureBox;
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

    triangleNoIndices->Load("./RenderMeshes/TriangleWithoutIndices.gltf");
    triangleIndices->Load("./RenderMeshes/Triangle.gltf");
    box->Load("./RenderMeshes/BoxInterleaved.gltf");
    textureBox->Load("../../ModelsTextures/BoxTextured.gltf");
    house->Load("../../ModelsTextures/BakerHouse.gltf");

    return true;
}

update_status ModuleRenderMeshes::Update(float deltaTime)
{
    //triangleNoIndices->Render(programNoTextures);
    //triangleIndices->Render(programNoTextures);
    //box->Render(programNoTextures);
    //textureBox->Render(programTextures);
    house->Render(programTextures);
    return UPDATE_CONTINUE;
}

bool ModuleRenderMeshes::CleanUp()
{
    if (programNoTextures > 0) glDeleteProgram(programNoTextures);
    if (programTextures > 0) glDeleteProgram(programTextures);

    return true;
}
