#include "DebugPainter.h"
#include "SDL/include/SDL_opengl.h"
#include "Globals.h"

void StartDebugDraw()
{
	glPushMatrix();

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
}

void EndDebugDraw()
{
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	glColor3f(1.0f, 1.0f, 1.0f);

	glPopMatrix();
}

void DebugDraw(const AABB &aabb, Color color)
{
	static vec vertices[8];
	aabb.GetCornerPoints(vertices);

	DebugDrawBox(vertices, color);
}

void DebugDraw(const OBB& obb, Color color)
{
	static float3 corners[8];
	obb.GetCornerPoints(corners);

	DebugDrawBox(corners, color);
}

void DebugDrawBox(const vec* vertices, Color color)
{
	glColor3f(color.r, color.g, color.b);

	glBegin(GL_LINES);

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

	glEnd();
}

void DebugDraw(const Frustum &frustum, Color color)
{
	float3 vertices[8];
	frustum.GetCornerPoints(vertices);

	DebugDrawBox(vertices, color);
}