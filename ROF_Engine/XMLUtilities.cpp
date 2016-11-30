#include "XMLUtilities.h"

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
