#ifndef __RENDERCOMP_H__
#define __RENDERCOMP_H__

#include "Globals.h"
#include "ComponentMesh.h"
#include "ComponentTransformation.h"
#include "ComponentMaterial.h"
#include "GameObject.h"

struct RenderComp
{
	GameObject* bearer;

	ComponentMesh* c_mesh = nullptr;
	ComponentTransformation* c_transform = nullptr;
	ComponentMaterial* c_material = nullptr;
};

#endif // !__RENDERCOMP_H__
