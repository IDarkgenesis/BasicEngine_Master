#pragma once

#include "Module.h"

class ImGuiIO;

class ModuleEditor : public Module
{
public:
	ModuleEditor();
	~ModuleEditor();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();

	void Draw();
	void ConfigMenu(bool& configMenu);
private:
	bool showcase = false;
	bool configMenu = false;
	bool checkBoxed = true;
	int value = 1;
};

