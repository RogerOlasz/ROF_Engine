#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransformation.h"
#include "ComponentCamera.h"
#include "RenderComp.h"
#include "DebugPainter.h"
#include <list>

GameObject::GameObject(const char* name) : name(name)
{
	transform = new ComponentTransformation(this, 0);
	have_camera = false;

	render_c = new RenderComp();
	render_c->c_transform = transform;
}

GameObject::~GameObject()
{
	if (this->parent != nullptr)
	{
		std::vector<GameObject*>::iterator tmp = this->parent->children.begin();
		while (tmp != this->parent->children.end())
		{
			if ((*tmp) == this)
			{
				this->parent->children.erase(tmp);
				break;
			}
			tmp++;
		}
	}

	if (children.size() != 0)
	{
		std::vector<GameObject*>::iterator it = children.begin();
		while (it != children.end() && children.size() > 0)
		{
			RELEASE((*it));

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

Component* GameObject::CreateComponent(Component::Type type)
{
	Component* new_component = nullptr;

	switch (type)
	{
	case Component::Type::Geometry:
		new_component = new ComponentMesh(this, components.size());
		render_c->c_mesh = (ComponentMesh*)new_component;
		break;
	case Component::Type::Material:
		new_component = new ComponentMaterial(this, components.size());
		render_c->c_material = (ComponentMaterial*)new_component;
		break;
	case Component::Type::Camera:
		if(this->GetComponentByType(Component::Type::Camera) == nullptr)
		{
			//TODO have camera
			new_component = new ComponentCamera(this, components.size());
			this->have_camera = true;
		}
		else
		{
			LOG("%s already have a camera.", GetName());
		}
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

void GameObject::Update()
{
	if (static_go == true)
	{
		transform->freeze = true;

		if (static_children)
		{
			if (children.size() != 0)
			{
				SetChildrenStatic(static_go);
			}
		}		
	}
	else
	{
		transform->freeze = false;
		if (parent != nullptr && children.size() != 0)
		{
			SetChildrenStatic(static_go);
		}
	}

	if (transform->global_matrix_changed)
	{
		UpdateGlobalMatrix();
		UpdateAABB();
		//TODO Mmmm must search other way to to this, this is so dirty
		if (have_camera)
		{
			((ComponentCamera*)GetComponentByType(Component::Type::Camera))->SetFront(&transform->GetGlobalMatrix());
			((ComponentCamera*)GetComponentByType(Component::Type::Camera))->SetUp(&transform->GetGlobalMatrix());
			((ComponentCamera*)GetComponentByType(Component::Type::Camera))->SetPos(&transform->GetGlobalMatrix());
		}
	}

	if (aabb_debug)
	{
		DebugDraw(bounding_box, Green);
	}
	if (obb_debug)
	{
		DebugDraw(go_obb, Turquoise);
	}

	for (std::vector<Component*>::iterator tmp = components.begin(); tmp != components.end(); tmp++)
	{
		(*tmp)->Update();
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
		if ((*comp)->GetType() == type)
		{
			return (*comp);
		}
		comp++;
	}
	return nullptr;
}

const AABB* GameObject::GetBoundingBox() const
{
	return &bounding_box;
}

const float4x4* GameObject::GetGlobalMatrixT() const
{
	return transform->GetGlobalMatrixT();
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

void GameObject::SetChildrenStatic(bool static_go)
{
	for (std::vector<GameObject*>::iterator tmp = children.begin(); tmp != children.end(); tmp++)
	{
		(*tmp)->static_go = static_go;
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

	return ret;
}

bool GameObject::Save(pugi::xml_node &scene)
{
	pugi::xml_node tmp_node;
	pugi::xml_node tmp_node_2;

	//Game objects basics
	tmp_node = scene.append_child("Name");
	tmp_node.text().set(name.c_str());

	tmp_node = scene.append_child("UUID");
	tmp_node.text().set(UUID);

	tmp_node = scene.append_child("ParentUUID");
	tmp_node.text().set(parent->UUID);

	//Components
	tmp_node = scene.append_child("Components");

	tmp_node_2 = tmp_node.append_child("Transformation");

	tmp_node_2 = tmp_node.append_child("Translate");
	tmp_node_2.append_attribute("X") = transform->GetPosition().x;
	tmp_node_2.append_attribute("Y") = transform->GetPosition().y;
	tmp_node_2.append_attribute("Z") = transform->GetPosition().z;

	tmp_node_2 = tmp_node.append_child("Scale");
	tmp_node_2.append_attribute("X") = transform->GetScale().x;
	tmp_node_2.append_attribute("Y") = transform->GetScale().y;
	tmp_node_2.append_attribute("Z") = transform->GetScale().z;

	tmp_node_2 = tmp_node.append_child("Rotation");
	tmp_node_2.append_attribute("X") = transform->GetRotation().x;
	tmp_node_2.append_attribute("Y") = transform->GetRotation().y;
	tmp_node_2.append_attribute("Z") = transform->GetRotation().z;

	if (this->GetComponentByType(Component::Type::Geometry))
	{
		tmp_node_2 = tmp_node.append_child("Mesh");
		tmp_node_2 = tmp_node.append_child("Type");
		tmp_node_2.text().set(this->GetComponentByType(Component::Type::Geometry)->GetType());
		tmp_node_2 = tmp_node;
	}
	
	if (this->GetComponentByType(Component::Type::Material))
	{
		tmp_node_2 = tmp_node.append_child("Material");
		tmp_node_2 = tmp_node.append_child("Type");
		tmp_node_2.text().set(this->GetComponentByType(Component::Type::Material)->GetType());
		tmp_node_2 = tmp_node.append_child("TexturePath");
		tmp_node_2.text().set(((ComponentMaterial*)this->GetComponentByType(Component::Type::Material))->tex_path.c_str());
		tmp_node_2 = tmp_node;
	}

	if (this->GetComponentByType(Component::Type::Camera))
	{
		tmp_node_2 = tmp_node.append_child("Camera");
		tmp_node_2 = tmp_node.append_child("Type");
		tmp_node_2.text().set(this->GetComponentByType(Component::Type::Camera)->GetType());
		tmp_node_2 = tmp_node;
	}	

	return true;
}