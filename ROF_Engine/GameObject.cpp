#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransformation.h"
#include <list>

GameObject::GameObject(const char* name, GameObject* parent) : name (name)
{
	SetParent(parent);
}

GameObject::~GameObject()
{

}

Component* GameObject::CreateComponent(Component::Types type)
{
	Component* new_component = nullptr;

	switch (type)
	{
	case Component::Types::Transformation:
		new_component = new ComponentTransformation(this);
		break;
	case Component::Types::Geometry:
		new_component = new ComponentMesh(this);
		break;
	case Component::Types::Material:
		new_component = new ComponentMaterial(this);
		break;
	}

	if (new_component != nullptr)
	{
		components.push_back(new_component);
	}		

	return new_component;
}

void GameObject::RemoveComponent(Component* to_delete)
{

}

bool GameObject::IsActive() const
{
	return active;
}

void GameObject::SwitchActive(bool active)
{
	active != active;
}

void GameObject::EnableComponent()
{
	for (std::list<Component*>::iterator tmp = components.begin(); tmp != components.end(); ++tmp)
	{
		(*tmp)->Activate();
	}		
}

void GameObject::DisableComponent()
{
	for (std::list<Component*>::iterator tmp = components.begin(); tmp != components.end(); ++tmp)
	{
		(*tmp)->Desactivate();
	}
}

void GameObject::Draw()
{

}

GameObject* GameObject::GetParent()
{
	return parent;
}

void GameObject::SetParent(GameObject* new_parent)
{
	if (new_parent == parent)
		return;

	if (parent)
	{
		parent->children.remove(this);
	}		

	parent = new_parent;

	if (new_parent)
	{
		new_parent->children.push_back(this);
	}
}

bool GameObject::Remove()
{
	bool ret = false;

	//Have to delete all component

	//Have to delete all children

	return ret;
}