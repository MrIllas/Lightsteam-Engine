#include <stdlib.h>
#include "Globals.h"
#include "Application.h"
#include "MemLeaks.h"

#include "External/SDL/include/SDL.h"


enum main_states
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

int main(int argc, char** argv)
{
	srand((unsigned)time(0));

	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;
	Application* App = nullptr;

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:

			LOG(LOG_TYPE::ENGINE, "-------------- Application Creation --------------");
			App = new Application();
			state = MAIN_START;
			break;

		case MAIN_START:

			LOG(LOG_TYPE::ENGINE, "-------------- Application Init --------------");
			if (App->Init() == false)
			{
				LOG(LOG_TYPE::ERRO, "Application Init exits with ERROR");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				//LOG("-------------- Application Update --------------");
				LOG(LOG_TYPE::ENGINE, "-------------- Engine Ready --------------");
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				LOG(LOG_TYPE::ERRO, "Application Update exits with ERROR");
				state = MAIN_EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;
		}
		break;

		case MAIN_FINISH:

			LOG(LOG_TYPE::ENGINE, "-------------- Application CleanUp --------------");
			if (App->CleanUp() == false)
			{
				LOG(LOG_TYPE::ERRO, "Application CleanUp exits with ERROR");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}
	}

	delete App;
	App = nullptr;

	ReportMemoryLeaks();

	LOG(LOG_TYPE::ENGINE, "\nBye :)\n");

	return main_return;
}