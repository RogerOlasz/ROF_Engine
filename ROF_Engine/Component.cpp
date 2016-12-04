#include "Component.h"
#include "GameObject.h"
#include "Resource.h"

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

const Resource* Component::GetResource() const
{
	return resource;
}

void Component::SetResource(Resource* resource)
{
	this->resource = resource;
	this->resource->on_use++;
}

void Component::UnsetResource(Resource* resource)
{
	//Sometimes recive from some c mesh a wrong pointer and crashes
	if (resource)
	{
		resource->on_use--;
		resource = nullptr;
	}
}