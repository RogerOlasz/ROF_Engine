#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"
#include "MathGeoLib/include/MathGeoLib.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);

	float2 GetWindowSize();

public:
	//The window we'll be rendering to
	SDL_Window* window;
	float2 window_size;
	//The surface contained by the window
	SDL_Surface* screen_surface;
};

#endif // __ModuleWindow_H__