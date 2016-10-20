#ifndef __DEBUGPAINTER_H__
#define __DEBUGPAINTER_H__

#include "Color.h"
#include "MathGeoLib/include/MathGeoLib.h"

void StartDebugDraw();
void EndDebugDraw();

void DebugDraw(const AABB &aabb, Color color = White, const float4x4 &transform = float4x4::identity);
void DebugDrawBox(const vec* vertices, Color color = White);
void DebugDraw(const Frustum &frustum, Color color = White);

#endif // !__DEBUGPAINTER_H__

