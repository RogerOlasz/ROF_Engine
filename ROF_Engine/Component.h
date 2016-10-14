#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <string>

class GameObject;

class Component
{
public:
	enum Types
	{
		Transformation,
		Geometry,
		Material,
		Unknown
	};

	std::string name;

public:
	Component(GameObject* bearer, Types type, int id);
	virtual ~Component();

	bool IsActive() const;

	virtual void Activate() {};
	virtual void Desactivate() {};

	Types GetType() const;
	int GetID() const;

private:
	bool active = false;
	Types type = Types::Unknown;
	GameObject* game_object = nullptr;

	int id;

};

#endif // !__COMPONENT_H__

