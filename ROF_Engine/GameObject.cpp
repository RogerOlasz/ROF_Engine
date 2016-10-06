#include "GameObject.h"
#include "Component.h"

GameObject::GameObject(GameObject* parent, const char* name)
{

}

GameObject::GameObject(GameObject* parent, const char* name, const float3& translation, const float3& scale, const Quat& rotation)
{

}

GameObject::~GameObject()
{

}

//Component* GameObject::CreateComponent(Component::Types type)
//{
//
//}

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