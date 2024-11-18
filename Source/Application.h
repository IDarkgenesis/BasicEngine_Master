#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleOpenGL;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleProgram;
class ModuleRenderExercise;
class ModuleDebugDraw;
class ModuleCamera;
class ModuleEditor;

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

    ModuleRenderExercise*  GetRenderExercise() { return renderExercise; }
    ModuleCamera* GetCamera() { return camera; }

private:

    ModuleOpenGL* render = nullptr;
    ModuleWindow* window = nullptr;
    ModuleInput* input = nullptr;
    ModuleProgram* program = nullptr;

    ModuleRenderExercise* renderExercise = nullptr;
    ModuleDebugDraw* debugDraw = nullptr;

    ModuleCamera* camera = nullptr;
    ModuleEditor* editor = nullptr;

    std::list<Module*> modules;

};

extern Application* App;
