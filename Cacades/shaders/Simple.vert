#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out Vertex
{
	vec3 position;
	vec3 normal;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  gl_Position	= projection * view * model * vec4(position, 1.0f);
  vs_out.position = position;
  vs_out.normal = normal;
}
