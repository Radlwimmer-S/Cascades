#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in Particle
{
    vec3 position;
    int type;
} gs_in[];

out ParticleQuad
{
    vec2 uv;
    flat int type;
} gs_out;

#pragma include "EnumParticleType.h"

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float size = 0.05f;

void RenderScreenOriented()
{
	vec3 up = vec3(0, 1, 0);
	 vec3 right = vec3(1, 0, 0);
	
    gl_Position = projection * (view * model * vec4(gs_in[0].position, 1.0f) + vec4(- up * size - right * size, 1.0f));
    gs_out.uv = vec2(0, 0);
    gs_out.type = gs_in[0].type;
    EmitVertex();
	
    gl_Position = projection * (view * model * vec4(gs_in[0].position, 1.0f) + vec4(- up * size + right * size, 1.0f));
    gs_out.uv = vec2(0, 1);
    gs_out.type = gs_in[0].type;
    EmitVertex();
	
    gl_Position = projection * (view * model * vec4(gs_in[0].position, 1.0f) + vec4(+ up * size - right * size, 1.0f));
    gs_out.uv = vec2(1, 0);
    gs_out.type = gs_in[0].type;
    EmitVertex();

    gl_Position = projection * (view * model * vec4(gs_in[0].position, 1.0f) + vec4(+ up * size + right * size, 1.0f));
    gs_out.uv = vec2(1, 1);
    gs_out.type = gs_in[0].type;
    EmitVertex();
    EndPrimitive();
}

void RenderGeometryOriented()
{
	RenderScreenOriented();
    //look = normal;
    //right = normal.yzx;
    //up = normal.zxy;
}

void main()
{
    switch(gs_in[0].type)
    {
    case PARTICLE_RAY: return;
      case PARTICLE_EMITTER:
      case PARTICLE_WATER_FLOWING: RenderGeometryOriented();
      break;
      case PARTICLE_WATER_FALLING:
      case PARTICLE_MIST_COLLISION:
      case PARTICLE_MIST_FALLING: RenderScreenOriented();
      break;
    }
}
