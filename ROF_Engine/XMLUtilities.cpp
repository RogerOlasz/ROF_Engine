#include "XMLUtilities.h"
#include "Color.h"

void AddXMLVector3(pugi::xml_node &node, vec to_add)
{
	node.append_attribute("X") = to_add.x;
	node.append_attribute("Y") = to_add.y;
	node.append_attribute("Z") = to_add.z;
}

vec GetXMLVector3(pugi::xml_node &node, const char* child_name, float default_value)
{
	vec tmp;

	tmp.x = node.child(child_name).attribute("X").as_float(default_value);
	tmp.y = node.child(child_name).attribute("Y").as_float(default_value);
	tmp.z = node.child(child_name).attribute("Z").as_float(default_value);

	return tmp;
}

void AddXMLColor(pugi::xml_node &node, Color color)
{
	node.append_attribute("Red") = color.r;
	node.append_attribute("Green") = color.g;
	node.append_attribute("Blue") = color.b;
	node.append_attribute("Alpha") = color.a;
}

Color GetXMLColor(pugi::xml_node &node, const char* child_name, float default_value)
{
	Color tmp;

	tmp.r = node.child(child_name).attribute("Red").as_float(default_value);
	tmp.g = node.child(child_name).attribute("Green").as_float(default_value);
	tmp.b = node.child(child_name).attribute("Blue").as_float(default_value);
	tmp.a = node.child(child_name).attribute("Alpha").as_float(default_value);

	return tmp;
}