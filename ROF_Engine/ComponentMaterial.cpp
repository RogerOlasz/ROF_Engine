#include "ComponentMaterial.h"

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

ComponentMaterial::ComponentMaterial(GameObject* bearer) : Component(bearer, Types::Geometry)
{

}

ComponentMaterial::~ComponentMaterial()
{

}

void ComponentMaterial::Init()
{
	//Initialize DevIL
	ilInit();
	iluInit();
	ilutInit();

	ilutRenderer(ILUT_OPENGL);
}