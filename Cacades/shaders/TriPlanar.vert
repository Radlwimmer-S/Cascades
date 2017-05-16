#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 uvw;

out VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec3 UVW;
    mat3 TBN;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position	= projection * view * model * vec4(position, 1.0f);
    vs_out.FragPos = position;
    vs_out.Normal = normal;
    vs_out.UVW = uvw;

    vec3 tangent = vec3(1, 0, 0);
    vec3 binormal = normalize(cross(tangent, normal));
    tangent = normalize(cross(normal, binormal));
    mat3 TBN = transpose(mat3(tangent, binormal, normal));

  	vs_out.TBN = TBN;
}
