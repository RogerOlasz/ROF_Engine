#include "Component.h"
#include "GameObject.h"

Component::Component(GameObject* bearer, Component::Types type, int id) : game_object(bearer), type(type), id(id)
{
	name = "Empty component";
}

Component::~Component()
{

}

bool Component::IsActive() const
{
	return active;
}

Component::Types Component::GetType() const
{
	return type;
}

int Component::GetID() const
{
	return id;
}