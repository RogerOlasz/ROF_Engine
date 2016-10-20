#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransformation.h"
#include "ComponentCamera.h"
#include "DebugPainter.h"
#include <list>

GameObject::GameObject(const char* name) : name(name)
{
	transform = new ComponentTransformation(this, 00);
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

	RELEASE(transform);
}

Component* GameObject::CreateComponent(Component::Types type)
{
	Component* new_component = nullptr;

	switch (type)
	{
	case Component::Types::Geometry:
		new_component = new ComponentMesh(this, components.size());
		break;
	case Component::Types::Material:
		new_component = new ComponentMaterial(this, components.size());
		break;
	case Component::Types::Camera:
		new_component = new ComponentCamera(this, components.size());
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
	if (transform->global_matrix_changed)
	{
		UpdateGlobalMatrix();
		UpdateAABB();
	}

	DebugDraw(bounding_box, Green);
	DebugDraw(go_obb, Red);

	transform->PushMatrix();

	for (std::vector<Component*>::iterator tmp = components.begin(); tmp != components.end(); tmp++)
	{
		(*tmp)->Update();
		
	}

	transform->PopMatrix();

	if (children.size() == 1)
	{
		(*children.begin())->Update();
	}
	else if (children.size() > 1)
	{
		for (std::vector<GameObject*>::iterator tmp = children.begin(); tmp != children.end(); tmp++)
		{
			(*tmp)->Update();
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

void GameObject::UpdateGlobalMatrix()
{
	for (std::vector<GameObject*>::iterator tmp = children.begin(); tmp != children.end(); tmp++)
	{
		(*tmp)->transform->UpdateGMatrix();
		(*tmp)->UpdateGlobalMatrix();
	}
}

void GameObject::UpdateAABB()
{
	for (std::vector<Component*>::iterator tmp = components.begin(); tmp != components.end(); tmp++)
	{
		if ((*tmp)->GetType() == Component::Types::Geometry)
		{
			const AABB aabb = *((ComponentMesh*)(*tmp))->GetBoundingBox();
			go_obb = aabb;
			go_obb.Transform(transform->GetGlobalMatrix());

			bounding_box.SetNegativeInfinity();
			bounding_box.Enclose(go_obb);
		}
	}
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

const Component* GameObject::GetComponentByType(Component::Types type)
{
	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i]->GetType() == type)
		{
			return components[i];
		}
	}

	return nullptr;
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