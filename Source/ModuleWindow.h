#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	// Called before quitting
	bool Init() override;

	// Called before quitting
	bool CleanUp() override;

	void ResizeWindow(int newWidth, int newHeight);
	void SetWindowResizable(SDL_bool resizable);
	void SetWindowFullscreen(Uint32 flag);
	void SetWindowBorderless(SDL_bool borderless);

public:
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;
};

#endif // __ModuleWindow_H__