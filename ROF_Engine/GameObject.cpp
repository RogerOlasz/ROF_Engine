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
	if (parent != nullptr)
	{
		std::vector<GameObject*>::iterator it = parent->children.begin();
		while ((*it) != this)
		{
			it++;
		}
		parent->children.erase(it);
	}
	
	if (children.empty() != true)
	{
		std::vector<GameObject*>::iterator it = children.begin();
		while (children.size() > 0 && it != children.end())
		{
			RELEASE(*it);
			if (children.size() > 0)
			{
				it = children.begin();
			}
		}
	}

	std::vector<Component*>::iterator comp = components.begin();
	while (comp != components.end())
	{
		(*comp)->CleanUp();
		RELEASE(*comp);
		comp++;
	}
}

Component* GameObject::CreateComponent(Component::Types type)
{
	Component* new_component = nullptr;

	switch (type)
	{
	case Component::Types::Transformation:
		new_component = new ComponentTransformation(this, components.size());
		break;
	case Component::Types::Geometry:
		new_component = new ComponentMesh(this, components.size());
		break;
	case Component::Types::Material:
		new_component = new ComponentMaterial(this, components.size());
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

void GameObject::Update()
{
	ComponentTransformation* tmp_t = nullptr;
	for (std::vector<Component*>::iterator tmp = components.begin(); tmp != components.end(); tmp++)
	{
		if ((*tmp)->GetType() == Component::Types::Transformation)
		{
			tmp_t = ((ComponentTransformation*)(*tmp));
			tmp_t->PushMatrix();
		}

		if ((*tmp)->GetType() == Component::Types::Geometry)
		{
			((ComponentMesh*)(*tmp))->Draw();
		}
	}

	if (children.size() == 1)
	{
		(*children.begin())->Update();
	}
	else if(children.size() > 1)
	{
		for (std::vector<GameObject*>::iterator tmp = children.begin(); tmp != children.end(); tmp++)
		{
			(*tmp)->Update();
		}
	}	
	tmp_t->PopMatrix();
}

GameObject* GameObject::GetParent()
{
	return parent;
}

const char* GameObject::GetName()
{
	return name.c_str();
}

std::string* GameObject::GetNameString()
{
	return &name;
}

void GameObject::SetName(const char* new_name)
{
	name = new_name;
}

bool GameObject::GetHierarchyState()
{ 
	return is_on_hierarchy;
}

void GameObject::SwitchParent(GameObject* new_parent)
{
	if (new_parent != nullptr)
	{
		if (parent != nullptr)
		{
			std::vector<GameObject*>::iterator it = parent->children.begin();
			while ((*it) != this)
			{
				it++;
			}
			parent->children.erase(it);
			
		}
		parent = new_parent;
		parent->children.push_back(this);
	}
}

bool GameObject::RemoveGameObject(GameObject* to_delete)
{
	bool ret = false;

	if (to_delete)
	{
		RELEASE(to_delete);
		ret = true;
	}

	return ret;
}