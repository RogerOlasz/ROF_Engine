#include "DebugPainter.h"
#include "SDL/include/SDL_opengl.h"
#include "Globals.h"

void DebugDraw(const AABB &aabb, Color color)
{
	vec vertices[8];
	aabb.GetCornerPoints(vertices);

	DebugDrawBox(vertices, color);
}

void DebugDraw(const OBB &obb, Color color)
{
	vec vertices[8];
	obb.GetCornerPoints(vertices);

	DebugDrawBox(vertices, color);
}

void DebugDraw(const Frustum &frustum, Color color)
{
	float3 vertices[8];
	frustum.GetCornerPoints(vertices);

	DebugDrawBox(vertices, color);
}

void DebugDrawBox(const vec* vertices, Color color)
{
	glColor3f(color.r, color.g, color.b);

	glBegin(GL_LINES);

#pragma region CubeFaces
	//Direct mode as boxes.
	
	//         4-----5
	//        /|    /|
	//       / |   / |
	//		0--6--1  7
	//		| /	  | /
	//		|/    |/
	//		2-----3
	
	glVertex3fv((GLfloat*)&vertices[1]);
	glVertex3fv((GLfloat*)&vertices[5]);
	glVertex3fv((GLfloat*)&vertices[7]);
	glVertex3fv((GLfloat*)&vertices[3]);

	glVertex3fv((GLfloat*)&vertices[4]);
	glVertex3fv((GLfloat*)&vertices[0]);
	glVertex3fv((GLfloat*)&vertices[2]);
	glVertex3fv((GLfloat*)&vertices[6]);

	glVertex3fv((GLfloat*)&vertices[5]);
	glVertex3fv((GLfloat*)&vertices[4]);
	glVertex3fv((GLfloat*)&vertices[6]);
	glVertex3fv((GLfloat*)&vertices[7]);

	glVertex3fv((GLfloat*)&vertices[0]);
	glVertex3fv((GLfloat*)&vertices[1]);
	glVertex3fv((GLfloat*)&vertices[3]);
	glVertex3fv((GLfloat*)&vertices[2]);

	glVertex3fv((GLfloat*)&vertices[1]);
	glVertex3fv((GLfloat*)&vertices[3]);
	glVertex3fv((GLfloat*)&vertices[0]);
	glVertex3fv((GLfloat*)&vertices[2]);

	glVertex3fv((GLfloat*)&vertices[5]);
	glVertex3fv((GLfloat*)&vertices[7]);
	glVertex3fv((GLfloat*)&vertices[4]);
	glVertex3fv((GLfloat*)&vertices[6]);
#pragma endregion

	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
}

