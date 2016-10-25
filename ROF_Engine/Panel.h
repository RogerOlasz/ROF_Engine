#ifndef __PANEL_H__
#define __PANEL_H__

#include "Globals.h"
#include <string>

class Panel
{
public:
	Panel(const char* name);
	virtual ~Panel();

	virtual void Draw();

	bool IsActive() const;

public:
	bool active = false;
	bool set_size = false;

protected:
	std::string name;

};

#endif // __PANEL_H__