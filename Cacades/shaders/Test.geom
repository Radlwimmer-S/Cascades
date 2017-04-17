#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3[] geoValue;
out vec3 out_position;

void main()
{
	for (int i = 0; i < 3; i++) {
		out_position = geoValue[0] + i;
		EmitVertex();
	}

	EndPrimitive();
}