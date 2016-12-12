#include "Component.h"
#include "GameObject.h"
#include "Resource.h"
#include "ResourceMaterial.h"
#include "ResourceTexture.h"
#include "ResourceMesh.h"

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

Resource* Component::GetResource() const
{
	return resource;
}

uint Component::GetResourceOnUse() const
{
	return resource->on_use;
}

void Component::SetResource(Resource* resource)
{
	this->resource = resource;
	this->resource->on_use++;
	using_resource = true;
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