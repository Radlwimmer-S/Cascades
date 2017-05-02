#version 430 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT
{
	int layer;
	vec3 position;
} gs_in[];

void main()
{
	for (int i = 0; i < 3; ++i)
	{
		gl_Layer = gs_in[i].layer;
		gl_Position = vec4(gs_in[i].position, 1.0f);
		EmitVertex();
	}
	EndPrimitive();
}