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
	void SwitchActive(bool active);
	void SetActive(bool active);

	virtual void OnActivate() {};
	virtual void OnDeActivate() {};

private:
	bool active = false;
	Types type = Types::Unknown;
	GameObject* game_object = nullptr;

};

#endif // !__COMPONENT_H__

