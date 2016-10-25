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
	active = true;
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

	children.clear();

	std::vector<Component*>::iterator comp = components.begin();
	while (comp != components.end())
	{
		(*comp)->CleanUp();
		RELEASE(*comp);
		comp++;
	}

	RELEASE(transform);
}

Component* GameObject::CreateComponent(Component::Type type)
{
	Component* new_component = nullptr;

	switch (type)
	{
	case Component::Type::Geometry:
		new_component = new ComponentMesh(this, components.size());
		break;
	case Component::Type::Material:
		new_component = new ComponentMaterial(this, components.size());
		break;
	case Component::Type::Camera:
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

void GameObject::SetActive(bool active)
{
	this->active = active;
}

void GameObject::Update()
{
	if (active)
	{
		if (static_go)
		{
			transform->freeze = true;
		}
		else
		{
			transform->freeze = false;
		}

		if (transform->global_matrix_changed)
		{
			UpdateGlobalMatrix();
			UpdateAABB();
		}

		if (aabb_debug)
		{
			DebugDraw(bounding_box, Green);
		}
		if (obb_debug)
		{
			DebugDraw(go_obb, Red);
		}

		transform->PushMatrix();
		for (std::vector<Component*>::iterator tmp = components.begin(); tmp != components.end(); tmp++)
		{
			(*tmp)->Update();

		}
		transform->PopMatrix();
	}
	else
	{
		LOG("%s have active = false.", name.c_str());
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

const Component* GameObject::GetComponentByType(Component::Type type)
{
	std::vector<Component*>::iterator comp = components.begin();
	while (comp != components.end())
	{
		if ((*comp)->GetID() == type)
		{
			return (*comp);
		}
		comp++;
	}
	return nullptr;
}

const AABB GameObject::GetBoundingBox() const
{
	return bounding_box;
}

void GameObject::SetName(const char* new_name)
{
	name = new_name;
}

void GameObject::UpdateGlobalMatrix()
{
	for (std::vector<GameObject*>::iterator tmp = children.begin(); tmp != children.end(); tmp++)
	{
		(*tmp)->transform->UpdateGlobalMatrix();
		(*tmp)->UpdateGlobalMatrix();
	}
}

void GameObject::UpdateAABB()
{
	for (std::vector<Component*>::iterator tmp = components.begin(); tmp != components.end(); tmp++)
	{
		if ((*tmp)->GetType() == Component::Type::Geometry)
		{
			const AABB aabb = *((ComponentMesh*)(*tmp))->GetBoundingBox();

			go_obb = aabb.ToOBB();
			go_obb.Transform(transform->GetGlobalMatrix());

			bounding_box.SetNegativeInfinity(); //Must be called before Enclose() to ser box at null
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