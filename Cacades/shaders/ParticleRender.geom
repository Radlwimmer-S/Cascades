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
    vec3 position;
    vec2 uv;
    int type;
} gs_out;

void main()
{
  
}
