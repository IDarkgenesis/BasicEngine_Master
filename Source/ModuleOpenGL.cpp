#include "Globals.h"
#include "Application.h"
#include "ModuleOpenGL.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include "glew-2.1.0/include/GL/glew.h"
#include "ModuleCamera.h"
#include "Windows.h"


ModuleOpenGL::ModuleOpenGL()
{
	context = nullptr;
}

// Destructor
ModuleOpenGL::~ModuleOpenGL()
{
}

// Called before render is available
bool ModuleOpenGL::Init()
{
	GLOG("Creating Renderer context");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	context = SDL_GL_CreateContext(App->GetWindowModule()->window);
	GLenum err = glewInit();

	GLOG("Using Glew %s", glewGetString(GLEW_VERSION));
	
	GLOG("Vendor: %s", glGetString(GL_VENDOR));
	GLOG("Renderer: %s", glGetString(GL_RENDERER));
	GLOG("OpenGL version supported %s", glGetString(GL_VERSION));
	GLOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glEnable(GL_DEPTH_TEST); // Enable depth test
	glEnable(GL_CULL_FACE); // Enable cull backward faces
	glFrontFace(GL_CCW);	// Enable conter clock wise backward faces

	return true;
}

update_status ModuleOpenGL::PreUpdate()
{
	int CurrentWidth = 0;
	int CurrentHeight = 0;
	SDL_GetWindowSize(App->GetWindowModule()->window, &CurrentWidth, &CurrentHeight);

	if (CurrentWidth && CurrentHeight) {
		glViewport(0,0, CurrentWidth, CurrentHeight);

		glClearColor(clearColorRed, clearColorGreen, clearColorBlue, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}


	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleOpenGL::Update(float deltaTime)
{

	return UPDATE_CONTINUE;
}

update_status ModuleOpenGL::PostUpdate()
{
	SDL_GL_SwapWindow(App->GetWindowModule()->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleOpenGL::CleanUp()
{
	GLOG("Destroying renderer");

	SDL_GL_DeleteContext(App->GetWindowModule()->window);

	return true;
}

void ModuleOpenGL::WindowResized(unsigned width, unsigned height)
{
	// Recalculating Vertical FOV -> important height / width
	float aspectRatio = (float)height / width;
	App->GetCameraModule()->SetAspectRatio(aspectRatio);
}

void ModuleOpenGL::SetClearRed(float newValue)
{
	clearColorRed = newValue;
}

void ModuleOpenGL::SetClearGreen(float newValue)
{
	clearColorGreen = newValue;
}

void ModuleOpenGL::SetClearBlue(float newValue)
{
	clearColorBlue = newValue;
}
