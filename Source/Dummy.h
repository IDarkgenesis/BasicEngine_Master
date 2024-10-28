#pragma once
#include "Module.h"
#include "Globals.h"

class ModuleDummy : public Module
{
	bool Init()
	{
		GLOG("Dummy Init!");
		return true;
	}

	bool CleanUp()
	{
		GLOG("Dummy CleanUp!");
		return true;
	}
};