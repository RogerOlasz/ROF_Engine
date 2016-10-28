#ifndef __DEBUGPAINTER_H__
#define __DEBUGPAINTER_H__

#include "Color.h"
#include "MathGeoLib/include/MathGeoLib.h"

//Debug draw prints as wireframe some basic geometry
void DebugDraw(const AABB &aabb, Color color = White);
void DebugDraw(const OBB &obb, Color color = White);
void DebugDraw(const Frustum &frustum, Color color = White);
void DebugDrawBox(const vec* vertices, Color color = White);

#endif // !__DEBUGPAINTER_H__

