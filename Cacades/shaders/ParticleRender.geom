#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in Particle
{
	int type;
	vec3 position;
	vec3 velocity;
} gs_in[];

out ParticleQuad
{
    flat int type;
    vec2 uv;
} gs_out;

#pragma include "EnumParticleType.h"

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform sampler3D normalTex;
uniform float screenSize = 0.1f;
uniform float geometrySize = 0.1f;

vec3 ws_to_UVW(vec3 ws)
{
    vec3 scaled = ws * 0.5f + 0.5f;
    return vec3(scaled.xz, scaled.y);
}

void RenderScreenOriented()
{
	vec3 up = vec3(0, 1, 0);
	vec3 right = vec3(1, 0, 0);

    gl_Position = projection * (view * model * vec4(gs_in[0].position, 1.0f) + vec4(- up * screenSize - right * screenSize, 1.0f));
    gs_out.uv = vec2(0, 0);
    gs_out.type = gs_in[0].type;
    EmitVertex();

    gl_Position = projection * (view * model * vec4(gs_in[0].position, 1.0f) + vec4(- up * screenSize + right * screenSize, 1.0f));
    gs_out.uv = vec2(0, 1);
    gs_out.type = gs_in[0].type;
    EmitVertex();

    gl_Position = projection * (view * model * vec4(gs_in[0].position, 1.0f) + vec4(+ up * screenSize - right * screenSize, 1.0f));
    gs_out.uv = vec2(1, 0);
    gs_out.type = gs_in[0].type;
    EmitVertex();

    gl_Position = projection * (view * model * vec4(gs_in[0].position, 1.0f) + vec4(+ up * screenSize + right * screenSize, 1.0f));
    gs_out.uv = vec2(1, 1);
    gs_out.type = gs_in[0].type;
    EmitVertex();
    EndPrimitive();
}

void RenderGeometryOriented()
{
	vec3 normal = normalize(texture(normalTex, ws_to_UVW(gs_in[0].position))).rgb;
	vec3 right = normal.yzx;
	vec3 forward = normal.zxy;

    //gl_Position = projection * (view * model * vec4(gs_in[0].position, 1.0f) + vec4(+ up * screenSize + right * screenSize, 1.0f));
	gl_Position = projection * (view * model * vec4(gs_in[0].position, 1.0f) + vec4(- forward * geometrySize - right * geometrySize, 1.0f));
    gs_out.uv = vec2(0, 0);
    gs_out.type = gs_in[0].type;
    EmitVertex();

	// gl_Position = projection * view * model * vec4(gs_in[0].position - forward * geometrySize + right * geometrySize, 1.0f);
	gl_Position = projection * (view * model * vec4(gs_in[0].position, 1.0f) + vec4(- forward * geometrySize + right * geometrySize, 1.0f));
    gs_out.uv = vec2(0, 1);
    gs_out.type = gs_in[0].type;
    EmitVertex();

	// gl_Position = projection * view * model * vec4(gs_in[0].position + forward * geometrySize - right * geometrySize, 1.0f);
	gl_Position = projection * (view * model * vec4(gs_in[0].position, 1.0f) + vec4(+ forward * geometrySize - right * geometrySize, 1.0f));
    gs_out.uv = vec2(1, 0);
    gs_out.type = gs_in[0].type;
    EmitVertex();

	// gl_Position = projection * view * model * vec4(gs_in[0].position + forward * geometrySize + right * geometrySize, 1.0f);
	gl_Position = projection * (view * model * vec4(gs_in[0].position, 1.0f) + vec4(+ forward * geometrySize + right * geometrySize, 1.0f));
    gs_out.uv = vec2(1, 1);
    gs_out.type = gs_in[0].type;
    EmitVertex();
    EndPrimitive();
}

void RenderError()
{
	vec3 up = vec3(0, 1, 0);
	vec3 right = vec3(1, 0, 0);

    gl_Position = projection * (view * model * vec4(gs_in[0].position, 1.0f) + vec4(- up * screenSize + right * screenSize, 1.0f));
    gs_out.uv = vec2(0, 1);
    gs_out.type = gs_in[0].type;
    EmitVertex();

    gl_Position = projection * (view * model * vec4(gs_in[0].position, 1.0f) + vec4(+ up * screenSize - right * screenSize, 1.0f));
    gs_out.uv = vec2(1, 0);
    gs_out.type = gs_in[0].type;
    EmitVertex();

    gl_Position = projection * (view * model * vec4(gs_in[0].position, 1.0f) + vec4(+ up * screenSize + right * screenSize, 1.0f));
    gs_out.uv = vec2(1, 1);
    gs_out.type = gs_in[0].type;
    EmitVertex();
    EndPrimitive();
}

void main()
{
    switch(gs_in[0].type)
    {
      case PARTICLE_RAY: return;
      case PARTICLE_EMITTER:
      case PARTICLE_WATER_FLOWING:  RenderGeometryOriented(); break;
      case PARTICLE_WATER_FALLING:
      case PARTICLE_MIST_COLLISION:
      case PARTICLE_MIST_FALLING:   RenderScreenOriented(); break;
      default: RenderError(); break;
    }
}
