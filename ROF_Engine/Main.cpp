#include <stdlib.h>
#include "Application.h"
#include "Globals.h"

#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )

// ------------- Include rules ------------------------

/*If, for example, class A uses class B, then class B is one of class A's
dependencies. Whether it can be forward declared or needs to be included
depends on how B is used within A:
-do nothing if : A makes no references at all to B
-do nothing if : The only reference to B is in a friend declaration
-forward declare B if : A contains a B pointer or reference : B* myb;
-forward declare B if: one or more functions has a B object / pointer / reference
as a parementer, or as a return type : B MyFunction(B myb);
-#include "b.h" if: B is a parent class of A
-#include "b.h" if : A contains a B object : B myb;*/

// ---------------- override specifier doc --------------------------

//http://en.cppreference.com/w/cpp/language/override

enum main_states
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

Application* App = NULL;

int main(int argc, char ** argv)
{
	LOG("Starting game '%s'...", TITLE);

	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:

			LOG("-------------- Application Creation --------------");
			App = new Application();
			state = MAIN_START;
			break;

		case MAIN_START:

			LOG("-------------- Application Init --------------");
			if (App->Init() == false)
			{
				LOG("Application Init exits with ERROR");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				LOG("-------------- Application Update --------------");
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				LOG("Application Update exits with ERROR");
				state = MAIN_EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;
		}
			break;

		case MAIN_FINISH:

			LOG("-------------- Application CleanUp --------------");
			if (App->CleanUp() == false)
			{
				LOG("Application CleanUp exits with ERROR");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}
	}

	LOG("Exiting game '%s'...\n", TITLE);
	delete App;

	return main_return;
}