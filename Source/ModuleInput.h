#pragma once
#include "Module.h"
#include "Globals.h"

#include "SDL/include/SDL_scancode.h"
#include "Math/float2.h"

typedef unsigned __int8 Uint8;
#define NUM_MOUSE_BUTTONS 5

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init();

	update_status PreUpdate();
	update_status Update();

	bool CleanUp();

	// Check key states (includes mouse and joy buttons)
	KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	KeyState GetMouseButtonDown(int id) const
	{
		return mouse_buttons[id - 1];
	}

	// Get mouse / axis position
	const float2& GetMouseMotion() const;
	const float2& GetMousePosition() const;

private:
	KeyState* keyboard = NULL;
	KeyState mouse_buttons[NUM_MOUSE_BUTTONS];
	float2 mouse_motion;
	float2 mouse;
};