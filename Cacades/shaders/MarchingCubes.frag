#version 330 core

in Vertex
{
	vec3 position;
	vec3 normal;
} fs_in;

out vec4 FragColor;

struct LightComponents
{
	float Ambient;
	float Diffuse;
	float Specular;
};

uniform vec3 viewPos;

LightComponents CalculateLight(in vec3 normal, in vec3 lightDir)
{
	LightComponents lighting;

	// Ambient
	float ambientStrength = 0.1f;
	lighting.Ambient = ambientStrength;

	// Diffuse
	float diffuseStrength = 1.0f;
	vec3 norm = normalize(normal);
	float diff = max(dot(norm, lightDir), 0.0);
	lighting.Diffuse = diffuseStrength * diff;

	// Specular
	float specularStrength = 0.1f;
	vec3 viewDir = normalize(viewPos - fs_in.position);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), 64.0);
	lighting.Specular = specularStrength * spec;

	return lighting;
}

void main()
{
	LightComponents light0 = CalculateLight(fs_in.normal, normalize(vec3(10, 10, -10) - fs_in.position));

	float l = light0.Ambient + light0.Diffuse + light0.Specular;
	vec3 lighting = vec3(l);

	lighting = clamp(lighting, 0, 1);

	FragColor = vec4(lighting, 1.0f);
}
