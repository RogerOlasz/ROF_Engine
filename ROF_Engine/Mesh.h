#ifndef __MESH_H__
#define __MESH_H__

#include "Globals.h"
#include "MathGeoLib\include\MathGeoLib.h"

//VRAM is graphic target's RAM

struct Mesh
{
	// Vertices
	uint id_vertices = 0; // id in VRAM
	uint num_vertices = 0;
	vec* vertices = nullptr;

	// Texture coords
	uint id_tex_coord = 0; // id in VRAM
	uint num_tex_coord = 0;
	float2* tex_coord = nullptr;

	// Normals
	uint id_normals = 0; // id in VRAM
	uint num_normals = 0;
	vec* normals = nullptr;

	// Indices
	uint id_indices = 0; // id in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;	
};

#endif // __MESH_H__
