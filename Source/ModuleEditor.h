#pragma once

#include "Module.h"

class ModuleEditor : public Module
{
public:
	ModuleEditor();
	~ModuleEditor();

	bool Init() override;
	update_status PreUpdate() override;
	update_status Update(float deltaTime) override;
	update_status PostUpdate() override;
	bool CleanUp() override;

	void Draw();
	void ConfigMenu(bool& configMenu);
	void Console(bool& console);

	void SetNewScreenSize(int newWidth, int newHeight);

private:
	bool showcaseMenu = false;
	bool configMenu = false;
	bool consoleMenu = false;
	
	bool closeApplication = false;

	// WINDOW VARIABLES
	int fov = HFOV;
	int width = SCREEN_WIDTH;
	int height = SCREEN_HEIGHT;
	bool resizableWindow = true;
	bool fullscreen = false;
	bool fullDesktop = false;
	bool borderless = false;

	// OpenGL Variables
	bool backFaceCulling = true;
	bool CCW_Face = true;
	bool depthTest = true;

	float clearRed = 0.5f;
	float clearGreen = 0.5f;
	float clearBlue = 0.5f;

	int maximumPlotData = 50;
	int currentPlotData = 0;
	std::vector<float> framerate;
	std::vector<float> frametime;
};

