#ifndef __PANELCOMPONENTS_H__
#define __PANELCOMPONENTS_H__

#include "Panel.h"
#include <string>
#include "MathGeoLib/include/MathGeoLib.h"

class GameObject;
class ComponentTransformation;
class ComponentMesh;
class ComponentMaterial;
class ComponentCamera;

class PanelComponents : public Panel
{
public:

	PanelComponents();
	~PanelComponents();

	void Draw(GameObject* selected_go);
	void DrawTransformation(ComponentTransformation* go_transform, GameObject* go_selected);
	void DrawMesh(ComponentMesh* go_mesh, GameObject* go_selected);
	void DrawMaterial(ComponentMaterial* go_material, GameObject* go_selected);
	void DrawCamera(ComponentCamera* go_camera, GameObject* go_selected);

private:
	GameObject* last_go = nullptr;

	vec pos = vec::zero;
	vec sca = vec::zero;
	vec rot = vec::zero;

	float near_plane;
	float far_plane;
	float field_of_view;
	float aspect_ratio;

	bool render_camera = false;
	bool static_ask = false;
};

#endif // !__PANELCOMPONENTS_H__

