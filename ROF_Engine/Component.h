#ifndef __COMPONENT_H__
#define __COMPONENT_H__

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

public:
	Component(GameObject* bearer, Types type);
	virtual ~Component();

	bool IsActive() const;
	void SetActive(bool active);

	virtual void Activate() {};
	virtual void Desactivate() {};

	virtual void Update() {};

	Types GetType() const;

private:
	bool active = false;
	Types type = Types::Unknown;
	GameObject* game_object = nullptr;

};

#endif // !__COMPONENT_H__

