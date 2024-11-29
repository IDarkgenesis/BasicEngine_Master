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
	bool CleanUp() override;

	void Draw();
	void ConfigMenu(bool& configMenu);
	void Console(bool& console);

private:
	bool showcase = false;
	bool configMenu = false;
	bool console = false;
	int fov = HFOV;

	int maximumPlotData = 50;
	int currentPlotData = 0;
	std::vector<float> framerate;
	std::vector<float> frametime;
};

