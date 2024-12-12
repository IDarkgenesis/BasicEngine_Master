#pragma once
#include "Module.h"
#include "Globals.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleOpenGL : public Module
{
public:
	ModuleOpenGL();
	~ModuleOpenGL();

	bool Init() override;
	update_status PreUpdate() override;
	update_status Update(float deltaTime) override;
	update_status PostUpdate() override;
	bool CleanUp() override;

	void WindowResized(unsigned width, unsigned height);

	void* GetContext() { return context; }

	void SetClearRed(float newValue);
	void SetClearGreen(float newValue);
	void SetClearBlue(float newValue);

private:
	void* context;

	float clearColorRed = DEFAULT_GL_CLEAR_COLOR_RED;
	float clearColorGreen = DEFAULT_GL_CLEAR_COLOR_GREEN;
	float clearColorBlue = DEFAULT_GL_CLEAR_COLOR_BLUE;
};
