#version 330 core
out vec4 FragColor;

in VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} fs_in;

struct PointLightSource
{
	vec3 Pos;
	vec3 Color;
	float far_plane;
	samplerCube depthMap;
};

uniform bool UsePointLights = true;
const int PointLightCount = 2;
uniform PointLightSource PointLight[PointLightCount];

struct DirLightSource
{
	vec3 Pos;
	vec3 Color;
	mat4 lightSpaceMatrix;
	sampler2D depthMap;
};

uniform bool UseDirLights = true;
const int DirLightCount = 1;
uniform DirLightSource DirLight[DirLightCount];

uniform int mode;
uniform vec3 objectColor;
uniform sampler2D objectTexture;

uniform vec3 viewPos;

vec3 DetermineFragmentColor(int mode)
{
	//Check for blending mode
	if (mode == -1)
	{
		return objectColor;
	}
	else if (mode == 1)
	{
		return texture(objectTexture, fs_in.TexCoords).rgb;
	}
	else
	{
		return texture(objectTexture, fs_in.TexCoords).rgb * objectColor;
	}
}

// array of offset direction for sampling
vec3 gridSamplingDisk[20] = vec3[]
(
	vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
	vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
	vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
	vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
	vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
	);

float CalculatePointShadow(vec3 fragPos, PointLightSource light)
{
	// Get vector between fragment position and light position
	vec3 fragToLight = fragPos - light.Pos;
	// Get current linear depth as the length between the fragment and light position
	float currentDepth = length(fragToLight);
	// Test for shadows with PCF
	float shadow = 0.0;
	float bias = 0.15;
	int samples = 20;
	float viewDistance = length(viewPos - fragPos);
	float diskRadius = (1.0 + (viewDistance / light.far_plane)) / 25.0f;
	for (int i = 0; i < samples; ++i)
	{
		float closestDepth = texture(light.depthMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
		closestDepth *= light.far_plane;   // Undo mapping [0;1]

		float distance = length(light.Pos - fs_in.FragPos);
		float distanceBias = max(bias, bias * distance / 5.0f);
		if (currentDepth - distanceBias > closestDepth)
			shadow += 1.0;
	}
	shadow /= float(samples);

	// Display closestDepth as debug (to visualize depth cubemap)
	// FragColor = vec4(vec3(closestDepth / far_plane), 1.0);    

	// return shadow;
	return shadow;
}

vec3 CalculatePointLight(vec3 fragPos, PointLightSource light)
{
	float distance = length(light.Pos - fs_in.FragPos);
	float distanceStrength = (1.0f - distance / light.far_plane);

	// Ambient
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * light.Color;

	// Diffuse 
	float diffuseStrength = 1.0f;
	vec3 norm = normalize(fs_in.Normal);
	vec3 lightDir = normalize(light.Pos - fs_in.FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diffuseStrength * diff * light.Color;

	// Specular
	float specularStrength = 0.1f;
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), 64.0);
	vec3 specular = specularStrength * spec * light.Color;

	// Calculate shadow
	float shadow = CalculatePointShadow(fs_in.FragPos, light);
	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * distanceStrength;

	return lighting;
}

float CalculateDirShadow(vec3 fragPos, DirLightSource light)
{
	vec4 fragPosLightSpace = light.lightSpaceMatrix * vec4(fragPos, 1.0f);

	// perform perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// Transform to [0,1] range
	projCoords = projCoords * 0.5 + 0.5;
	// Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
	float closestDepth = texture(light.depthMap, projCoords.xy).r;
	// Get depth of current fragment from light's perspective
	float currentDepth = projCoords.z;
	// Calculate bias (based on depth map resolution and slope)
	vec3 normal = normalize(fs_in.Normal);
	vec3 lightDir = normalize(-light.Pos);
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	// Check whether current frag pos is in shadow
	// float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
	// PCF
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(light.depthMap, 0);
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(light.depthMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;

	// Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
	if (projCoords.z > 1.0)
		shadow = 0.0;

	return shadow;
}

vec3 CalculateDirLight(vec3 fragPos, DirLightSource light)
{
	// Ambient
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * light.Color;

	// Diffuse 
	float diffuseStrength = 1.0f;
	vec3 norm = normalize(fs_in.Normal);
	vec3 lightDir = normalize(-light.Pos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diffuseStrength * diff * light.Color;

	// Specular
	float specularStrength = 0.1f;
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), 64.0);
	vec3 specular = specularStrength * spec * light.Color;

	// Calculate shadow
	float shadow = CalculateDirShadow(fs_in.FragPos, light);
	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular));

	return lighting;
}

void main()
{
	vec3 color = DetermineFragmentColor(mode);

	//No Normals --> no lighting
	if (fs_in.Normal == vec3(0.0f, 0.0f, 0.0f))
	{
		FragColor = vec4(color, 1.0f);
		return;
	}

	vec3 lighting = vec3(0.0f, 0.0f, 0.0f);
	if (UsePointLights)
		for (int i = 0; i < PointLightCount; i++)
		{
			lighting += CalculatePointLight(fs_in.FragPos, PointLight[i]);
		}

	if (UseDirLights)
		for (int i = 0; i < DirLightCount; i++)
		{
			lighting += CalculateDirLight(fs_in.FragPos, DirLight[i]);
		}

	lighting = clamp(lighting, 0, 1);
	lighting *= color;

	FragColor = vec4(lighting, 1.0f);
}