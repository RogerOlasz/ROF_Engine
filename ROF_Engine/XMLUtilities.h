#ifndef __XMLUTILITIES_H__
#define __XMLUTILITIES_H__

#include "MathGeoLib/include/MathGeoLib.h"
#include "PugiXML/src/pugixml.hpp"

class Color;

void AddXMLVector3(pugi::xml_node &node, vec to_add);
vec GetXMLVector3(pugi::xml_node &node, const char* child_name, float default_value = 0.0f);

void AddXMLColor(pugi::xml_node &node, Color color);
Color GetXMLColor(pugi::xml_node &node, const char* child_name, float default_value = 1.0f);


#endif // !__XMLUTILITIES_H__
