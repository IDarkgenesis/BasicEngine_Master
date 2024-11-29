#include <stdlib.h>
#include "Application.h"
#include "ModuleOpenGL.h"
#include "Globals.h"

#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/lib/x64/SDL2.lib" )
#pragma comment( lib, "SDL/lib/x64/SDL2main.lib" )

enum main_states
{
	MAIN_CREATION,  
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

Application* App = NULL;
std::vector<char*>* Logs = NULL;

int main(int argc, char ** argv)
{
	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;
	
	Logs = new std::vector<char*>();

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:

			GLOG("----- Application Creation -----");
			App = new Application();
			state = MAIN_START;
			break;

		case MAIN_START:

			GLOG("----- Application Init -----");
			if (App->Init() == false)
			{
				GLOG("----- Application Init exits with error -----");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				GLOG("----- Application Update -----");
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				GLOG("----- Application Update exits with error -----");
				state = MAIN_EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;
		}
			break;

		case MAIN_FINISH:

			GLOG("----- Application CleanUp -----");
			if (App->CleanUp() == false)
			{
				GLOG("----- Application CleanUp exits with error -----");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}

	}

	delete App;
	GLOG("Bye :)\n");

	// Free memory from log*
	for (auto log : *Logs)
	{
		free(log);
	}

	Logs->clear();
	delete Logs;

	return main_return;
}
