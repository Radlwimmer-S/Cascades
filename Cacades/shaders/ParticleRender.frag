#version 330 core

in ParticleQuad
{
    vec2 uv;
    flat int type;
} fs_in;

out vec4 FragColor;

#pragma include "EnumParticleType.h"

void main()
{
    if (fs_in.type == PARTICLE_EMITTER)
        FragColor = vec4(1, 0, 0, 1);
    else
        FragColor = vec4(0, 0, 1, 1);
}
