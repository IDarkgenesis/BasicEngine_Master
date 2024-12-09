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

class ModuleModelViewer;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

    ModuleOpenGL* GetOpenGLModule() { return render; }
    ModuleWindow* GetWindowModule() { return window; }
    ModuleInput*  GetInputModule() { return input; }
    ModuleProgram*  GetProgramModule() { return program; }
    ModuleTexture*  GetTextureModule() { return texture; }

    ModuleCamera* GetCameraModule() { return camera; }

    ModuleEditor* GetEditorModule() { return editor; }

    ModuleModelViewer* GetModelViewerModule() { return modelViewer; }

private:

    ModuleOpenGL* render = nullptr;
    ModuleWindow* window = nullptr;
    ModuleInput* input = nullptr;
    ModuleProgram* program = nullptr;
    ModuleTexture* texture = nullptr;

    ModuleCamera* camera = nullptr;
    ModuleEditor* editor = nullptr;

    ModuleDebugDraw* debugDraw = nullptr;
    ModuleModelViewer* modelViewer = nullptr;

    std::list<Module*> modules;

    uint32_t previousElapsedTime = 0;
};

extern Application* App;
