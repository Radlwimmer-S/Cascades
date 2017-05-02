#version 430 core
layout(location = 0) in vec3 position;

out VS_OUT
{
	int layer;
	vec3 position;
} vs_out;

void main()
{
	vs_out.layer = gl_InstanceID;
}