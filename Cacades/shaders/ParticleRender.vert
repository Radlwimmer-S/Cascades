#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 velocity;
layout(location = 2) in float lifeTime;
layout(location = 3) in float seed;
layout(location = 4) in int type;

out Particle
{
	vec3 position;
	int type;
} vs_out;

void main()
{
	vs_out.position = position;
	vs_out.type = type;
}
