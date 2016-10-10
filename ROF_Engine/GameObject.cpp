#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransformation.h"
#include <list>

GameObject::GameObject(const char* name) : name (name)
{

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
	if (children.size() == 1)
	{
		(*children.begin())->Draw();
	}
	else if(children.size() > 1)
	{
		for (std::list<GameObject*>::iterator tmp = children.begin(); tmp != children.end(); tmp++)
		{
			(*tmp)->Draw();
		}
	}

	for (std::list<Component*>::iterator tmp = components.begin(); tmp != components.end(); tmp++)
	{
		ComponentMesh* debug = ((ComponentMesh*)(*tmp));
		if ((*tmp)->GetType() == Component::Types::Geometry)
		{
			((ComponentMesh*)(*tmp))->Draw();
		}
	}
}

GameObject* GameObject::GetParent()
{
	return parent;
}

const char* GameObject::GetName()
{
	return name.c_str();
}

void GameObject::SwitchParent(GameObject* new_parent)
{
	if (new_parent != nullptr)
	{
		if (parent != nullptr)
		{
			parent->children.remove(this);
		}
		parent = new_parent;
		parent->children.push_back(this);
	}
}

bool GameObject::Remove()
{
	bool ret = false;

	//Have to delete all component

	//Have to delete all children

	return ret;
}