#ifndef __XMLUTILITIES_H__
#define __XMLUTILITIES_H__

#include "MathGeoLib/include/MathGeoLib.h"
#include "PugiXML/src/pugixml.hpp"

void AddXMLVector3(pugi::xml_node &node, vec to_add);
vec GetXMLVector3(pugi::xml_node &node, const char* child_name, float default_value = 0.0f);


#endif // !__XMLUTILITIES_H__
