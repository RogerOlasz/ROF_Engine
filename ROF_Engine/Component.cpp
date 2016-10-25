#include "Component.h"
#include "GameObject.h"

Component::Component(GameObject* bearer, Component::Type type, int id) : game_object(bearer), type(type), id(id)
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

Component::Type Component::GetType() const
{
	return type;
}

int Component::GetID() const
{
	return id;
}