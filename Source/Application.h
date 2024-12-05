#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleOpenGL;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleProgram;
class ModuleDebugDraw;
class ModuleCamera;
class ModuleEditor;
class ModuleTexture;
class ModuleTexture;

class ModuleRenderExercise;
class ModuleRenderMeshes;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

    ModuleOpenGL* GetOpenGL() { return render; }
    ModuleWindow* GetWindow() { return window; }
    ModuleInput*  GetInput() { return input; }
    ModuleProgram*  GetProgram() { return program; }
    ModuleTexture*  GetTexture() { return texture; }

    ModuleRenderExercise*  GetRenderExercise() { return renderExercise; }
    ModuleCamera* GetCamera() { return camera; }

    ModuleEditor* GetEditor() { return editor; }

private:

    ModuleOpenGL* render = nullptr;
    ModuleWindow* window = nullptr;
    ModuleInput* input = nullptr;
    ModuleProgram* program = nullptr;
    ModuleTexture* texture = nullptr;

    ModuleDebugDraw* debugDraw = nullptr;

    ModuleCamera* camera = nullptr;
    ModuleEditor* editor = nullptr;

    ModuleRenderExercise* renderExercise = nullptr;
    ModuleRenderMeshes* renderMeshesExercise = nullptr;

    std::list<Module*> modules;

    uint32_t previousElapsedTime = 0;
};

extern Application* App;
