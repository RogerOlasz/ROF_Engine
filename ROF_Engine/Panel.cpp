#include "Panel.h"

Panel::Panel(const char* name)
{

}

Panel::~Panel()
{

}

void Panel::SwitchActive()
{
	active = !active;
}

bool Panel::IsActive() const
{
	return active;
}

void Panel::Draw()
{

}

