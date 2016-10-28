#version 330 core
out vec4 color;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform int mode;
uniform vec3 objectColor;
uniform sampler2D objectTexture;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

void main()
{
	vec4 FragColor;

	if (mode == -1)
	{
		FragColor = vec4(objectColor, 1.0f);
	}
	else if (mode == 1)
	{
		FragColor = texture(objectTexture, TexCoord);
	}
	else
	{
		FragColor = texture(objectTexture, TexCoord) * vec4(objectColor, 1.0f);
	}

	if (Normal == vec3(0.0f, 0.0f, 0.0f))
	{
		color = FragColor;
		return;
	}

	// Ambient
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;

	// Diffuse 
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// Specular
	float specularStrength = 0.1f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
	vec3 specular = specularStrength * spec * lightColor;

	color = vec4(ambient + diffuse + specular, 1.0f) * FragColor;
}