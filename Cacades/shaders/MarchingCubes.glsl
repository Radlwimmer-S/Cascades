#version 440 core
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

struct Vertex 
{
	vec3 position;
	vec3 normal;
	vec2 uv;
	vec3 tangent;
};

layout(std430, binding = 0) buffer SSBO {
Vertex vertices[];
};

uniform vec3 resolution;
uniform vec3 scale;
uniform int isoLevel = 0;
uniform sampler3D tex;

struct CellTriangles 
{
	int tris[16];
}; 

layout (shared, std140) uniform MC_EdgeTable
{
	int edgeTable[256];
};
layout (shared, std140) uniform MC_TrisTable
{
	CellTriangles triTable[256];
};

struct Gridcell 
{
   vec3 p[8];
   float val[8];
   int mc_case;
};

vec3 ws_to_UVW(vec3 ws)
{
	return ws * 0.5f + 0.5f;
	//return (vec3(ws.xz*0.5+0.5, ws.y * WorldSpaceVolumeHeight).xzy);
}

Gridcell ConstructCell(vec3 position)
{
	Gridcell cell;

	//TODO: Check this
	cell.p[0] = position + (resolution * vec3(-0.5f, -0.5f, -0.5f));
	cell.p[1] = position + (resolution * vec3(+0.5f, -0.5f, -0.5f));
	cell.p[2] = position + (resolution * vec3(+0.5f, -0.5f, +0.5f));
	cell.p[3] = position + (resolution * vec3(-0.5f, -0.5f, +0.5f));
	cell.p[4] = position + (resolution * vec3(-0.5f, +0.5f, -0.5f));
	cell.p[5] = position + (resolution * vec3(+0.5f, +0.5f, -0.5f));
	cell.p[6] = position + (resolution * vec3(+0.5f, +0.5f, +0.5f));
	cell.p[7] = position + (resolution * vec3(-0.5f, +0.5f, +0.5f));


	for (int i = 0; i < 8; ++i)
	{
		cell.val[i] = texture(tex, ws_to_UVW(cell.p[i])).r;
	}

	/*
      Determine the index into the edge table which
      tells us which vertices are inside of the surface
    */
    int cubeindex = 0;
    if (cell.val[0] <= isoLevel) cubeindex |= 1;
    if (cell.val[1] <= isoLevel) cubeindex |= 2;
    if (cell.val[2] <= isoLevel) cubeindex |= 4;
    if (cell.val[3] <= isoLevel) cubeindex |= 8;
    if (cell.val[4] <= isoLevel) cubeindex |= 16;
    if (cell.val[5] <= isoLevel) cubeindex |= 32;
    if (cell.val[6] <= isoLevel) cubeindex |= 64;
    if (cell.val[7] <= isoLevel) cubeindex |= 128;

   /* Cube is entirely in/out of the surface */
    if (cubeindex == 0 || cubeindex == 255 ) {
      //TODO: Code something smart
	}

	cell.mc_case = cubeindex;
}


void main()
{
	uvec3 index = gl_GlobalInvocationID;

	if (index.x >= resolution.x || index.y >= resolution.y || index.z > resolution.z)
	return;

	vec3 position = vec3(-1) + resolution/2.0f + (resolution * index);

	Gridcell cell = ConstructCell(position);

}