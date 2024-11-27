#pragma once

#include "Module.h"

class ImGuiIO;

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
private:
	bool showcase = false;
	bool configMenu = false;
	bool checkBoxed = true;
	int value = 1;
};

