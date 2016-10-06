#include "Component.h"
#include "GameObject.h"

Component::Component(GameObject* bearer, Component::Types type) : game_object(bearer), type(type)
{
	if (game_object != nullptr)
	{
		SetActive(true);
	}
}

Component::~Component()
{

}

bool Component::IsActive() const
{
	return active;
}

void Component::SwitchActive(bool active)
{
	active != active;
}

void Component::SetActive(bool active)
{
	if (this->active != active)
	{
		this->active = active;
		if (active)
			OnActivate();
		else
			OnDeActivate();
	}
}