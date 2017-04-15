#version 330 core
layout(location = 0) in vec3 position;

float WorldSpaceVolumeHeight = 2.0f * (256.0f / 96.0f);
vec3 voxelDim = vec3(96, 256, 96);
vec3 voxelDimMinusOne = vec3(95, 256, 95);
vec3 wsVoxelSize = vec3(2.0f/95.0f);
vec4 inv_voxelDim = vec4( 1.0/voxelDim, 0);
vec4 inv_voxelDimMinusOne = vec4( 1.0/voxelDimMinusOne, 0 );
// This vertex shader is fed 95*95points, one for each *cell* we'll run M.C. on.
// To generate > 1 slice in a single frame, we call DrawInstanced(N),
// and it repeats it N times, each time setting nInstanceIDto [0 .. N-1].

uniform vec3 resolution;
uniform int isoLevel = 0;
uniform sampler3D tex;

out struct Gridcell {
   vec3 p[8];
   float val[8];
   int mc_case;
} vs_out;

vec3 ws_to_UVW(vec3 ws)
{
	return ws * 0.5f + 0.5f;
	//return (vec3(ws.xz*0.5+0.5, ws.y * WorldSpaceVolumeHeight).xzy);
}

void main()
{
	//TODO: Check this
	vs_out.p[0] = position + (resolution * vec3(-0.5f, -0.5f, -0.5f));
	vs_out.p[1] = position + (resolution * vec3(+0.5f, -0.5f, -0.5f));
	vs_out.p[2] = position + (resolution * vec3(+0.5f, -0.5f, +0.5f));
	vs_out.p[3] = position + (resolution * vec3(-0.5f, -0.5f, +0.5f));
	vs_out.p[4] = position + (resolution * vec3(-0.5f, +0.5f, -0.5f));
	vs_out.p[5] = position + (resolution * vec3(+0.5f, +0.5f, -0.5f));
	vs_out.p[6] = position + (resolution * vec3(+0.5f, +0.5f, +0.5f));
	vs_out.p[7] = position + (resolution * vec3(-0.5f, +0.5f, +0.5f));


	for (int i = 0; i < 8; ++i)
	{
		vs_out.val[i] = texture(tex, ws_to_UVW(vs_out.p[i])).r;
	}

	/*
      Determine the index into the edge table which
      tells us which vertices are inside of the surface
    */
    int cubeindex = 0;
    if (vs_out.val[0] <= isoLevel) cubeindex |= 1;
    if (vs_out.val[1] <= isoLevel) cubeindex |= 2;
    if (vs_out.val[2] <= isoLevel) cubeindex |= 4;
    if (vs_out.val[3] <= isoLevel) cubeindex |= 8;
    if (vs_out.val[4] <= isoLevel) cubeindex |= 16;
    if (vs_out.val[5] <= isoLevel) cubeindex |= 32;
    if (vs_out.val[6] <= isoLevel) cubeindex |= 64;
    if (vs_out.val[7] <= isoLevel) cubeindex |= 128;

   /* Cube is entirely in/out of the surface */
    if (cubeindex == 0 || cubeindex == 255 ) {
      //TODO: Code something smart
	}

	vs_out.mc_case = cubeindex;

	gl_Position = vec4(position, 1.0f); 
}