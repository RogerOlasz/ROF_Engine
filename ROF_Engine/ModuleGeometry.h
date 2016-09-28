#ifndef __ModuleGeometry_H__
#define __ModuleGeometry_H__

#include "Module.h"

class ModuleGeometry : public Module
{
public:

	ModuleGeometry(Application* app, bool start_enabled = true);
	~ModuleGeometry();

	bool Init();
	bool CleanUp();

private:

};

#endif // __ModuleGeometry_H__