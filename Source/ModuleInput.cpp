#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleOpenGL.h"
#include "SDL/include/SDL.h"
#include <imgui_impl_sdl2.h>
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "ModuleEditor.h"


#define MAX_KEYS 300

ModuleInput::ModuleInput() : Module(), mouse(float2::zero), mouse_motion(float2::zero)
{
    keyboard = new KeyState[MAX_KEYS];
    memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KeyState) * NUM_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
    RELEASE_ARRAY(keyboard);
}

// Called before render is available
bool ModuleInput::Init()
{
	GLOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		GLOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

update_status ModuleInput::PreUpdate()
{
	// Checking and updating keyboard key states
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	mouse_motion.x = 0;
	mouse_motion.y = 0;
	mouseWheel = 0;
	
	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (mouse_buttons[i] == KEY_DOWN)
			mouse_buttons[i] = KEY_REPEAT;

		if (mouse_buttons[i] == KEY_UP)
			mouse_buttons[i] = KEY_IDLE;
	}

	SDL_Event sdlEvent;


	while (SDL_PollEvent(&sdlEvent) != 0)
	{
		ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
		switch (sdlEvent.type)
		{
		case SDL_QUIT:
			return UPDATE_STOP;
		case SDL_WINDOWEVENT:
			if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED || sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
			{
				App->GetOpenGLModule()->WindowResized(sdlEvent.window.data1, sdlEvent.window.data2);
				App->GetEditorModule()->SetNewScreenSize(sdlEvent.window.data1, sdlEvent.window.data2);
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			mouse_buttons[sdlEvent.button.button - 1] = KEY_DOWN;
			break;

		case SDL_MOUSEBUTTONUP:
			mouse_buttons[sdlEvent.button.button - 1] = KEY_UP;
			break;
		case SDL_MOUSEMOTION:
			mouse_motion.x = sdlEvent.motion.xrel / 2.f;
			mouse_motion.y = sdlEvent.motion.yrel / 2.f;

			mouse.x = sdlEvent.motion.x / 2.f;
			mouse.y = sdlEvent.motion.y / 2.f;
			break;
		case SDL_MOUSEWHEEL:
			mouseWheel = sdlEvent.wheel.y;
			break;
		}
	}

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleInput::Update(float deltaTime)
{
    return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	GLOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

const float2& ModuleInput::GetMouseMotion() const
{
	return mouse_motion;
}

const float2& ModuleInput::GetMousePosition() const
{
	return mouse;
}
