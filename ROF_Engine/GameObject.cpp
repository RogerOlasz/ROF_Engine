#include "GameObject.h"

GameObject::GameObject(const char* name, GameObject* parent) : name (name)
{
	SetNewParent(parent);
}

GameObject::~GameObject()
{

}

Component* GameObject::CreateComponent(Component::Types type)
{
	Component* new_component = nullptr;

	/*switch (type)
	{
	case Component::Types::Transformation:
		new_component = new ComponentMesh(this);
		break;
	case Component::Types::Geometry:
		new_component = new ComponentMesh(this);
		break;
	case Component::Types::Material:
		new_component = new ComponentMaterial(this);
		break;
	}*/

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

void GameObject::Draw()
{

}

void GameObject::SetNewParent(GameObject* new_parent)
{
	if (new_parent == parent)
		return;

	if (parent)
	{
		parent->childs.remove(this);
	}		

	parent = new_parent;

	if (new_parent)
	{
		new_parent->childs.push_back(this);
	}
}