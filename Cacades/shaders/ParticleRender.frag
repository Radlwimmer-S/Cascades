#version 330 core

in ParticleQuad
{
    flat int type;
    vec2 uv;
} fs_in;

out vec4 FragColor;

#pragma include "EnumParticleType.h"

void main()
{
    if (fs_in.type == PARTICLE_EMITTER)
        FragColor = vec4(1, 0, 0, 1);
    else
        FragColor = vec4(0, 0, 1, 1);

    if (fs_in.type < -1 )
        FragColor = vec4(-fs_in.type, 0, 0, 1);

    if (fs_in.type > 4)
        FragColor = vec4(0, fs_in.type, 0, 1);
}
