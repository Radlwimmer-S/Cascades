#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 velocity;
layout(location = 2) in vec2 seed;
layout(location = 3) in float lifeTime;
layout(location = 4) in int type;

//struct Particle
//{
//	vec3 position;
//	int type;
//};
//out Particle vs_out;

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
