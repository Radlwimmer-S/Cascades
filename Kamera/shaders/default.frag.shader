#version 330 core
out vec4 FragColor;

in VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} fs_in;

struct LightComponents
{
	float Ambient;
	float Diffuse;
	float Specular;
};

struct PointLightSource
{
	vec3 Pos;
	vec3 Color;
	float far_plane;
	samplerCube depthMap;
};

uniform bool UsePointLights = false;
const int PointLightCount = 2;
uniform PointLightSource PointLight[PointLightCount];

struct DirLightSource
{
	vec3 Pos;
	vec3 Color;
	mat4 lightSpaceMatrix;
	sampler2D depthMap;
};

uniform bool UseDirLights = false;
const int DirLightCount = 1;
uniform DirLightSource DirLight[DirLightCount];

uniform bool UseSpotLights = true;
const int SpotLightCount = 1;
uniform DirLightSource SpotLight[SpotLightCount];

uniform int mode;
uniform vec3 objectColor;
uniform sampler2D objectTexture;

uniform vec3 viewPos;

uniform bool SoftShadows = true;

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

float CalculatePointShadow(PointLightSource light)
{
	// Get vector between fragment position and light position
	vec3 fragToLight = fs_in.FragPos - light.Pos;
	// Get current linear depth as the length between the fragment and light position
	float currentDepth = length(fragToLight);
	// Test for shadows with PCF
	float shadow = 0.0;
	float bias = 0.15;
	int samples = 20;
	float viewDistance = length(viewPos - fs_in.FragPos);
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

LightComponents CalculatePointLight(PointLightSource light)
{
	LightComponents lighting;

	// Ambient
	float ambientStrength = 0.1f;
	lighting.Ambient = ambientStrength;

	// Diffuse 
	float diffuseStrength = 1.0f;
	vec3 norm = normalize(fs_in.Normal);
	vec3 lightDir = normalize(light.Pos - fs_in.FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	lighting.Diffuse = diffuseStrength * diff;

	// Specular
	float specularStrength = 0.1f;
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), 64.0);
	lighting.Specular = specularStrength * spec;

	return lighting;
}

float CalculateDirShadow(DirLightSource light, float baseBias)
{
	vec4 fragPosLightSpace = light.lightSpaceMatrix * vec4(fs_in.FragPos, 1.0f);

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
	vec3 lightDir = normalize(light.Pos);

	float bias = max(baseBias / 10 * (1.0 - dot(normal, lightDir)), baseBias);

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

LightComponents CalculateDirLight(DirLightSource light)
{		
	LightComponents lighting;

	// Ambient
	float ambientStrength = 0.1f;
	lighting.Ambient = ambientStrength;

	// Diffuse 
	float diffuseStrength = 1.0f;
	vec3 norm = normalize(fs_in.Normal);
	vec3 lightDir = normalize(light.Pos);
	float diff = max(dot(norm, lightDir), 0.0);
	lighting.Diffuse = diffuseStrength * diff;

	// Specular
	float specularStrength = 0.1f;
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), 64.0);
	lighting.Specular = specularStrength * spec;

	return lighting;	
}

float CalculateCircularShadow(DirLightSource light)
{
	vec4 fragPosLightSpace = light.lightSpaceMatrix * vec4(fs_in.FragPos, 1.0f);

	// perform perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

	//Generate Circular Shadow
	float distanceToCenter = length(projCoords.xy);
	return clamp(distanceToCenter * distanceToCenter, 0, 1);
}

float CalculateDistanceShadow(PointLightSource light)
{
	float distance = length(light.Pos - fs_in.FragPos);
	return distance / light.far_plane;
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
			LightComponents light = CalculatePointLight(PointLight[i]);
			float shadow = CalculatePointShadow(PointLight[i]);
			float distance = CalculateDistanceShadow(PointLight[i]);
			shadow = clamp(shadow + distance, 0, 1);
			lighting += (light.Ambient + (1.0 - shadow) * (light.Diffuse + light.Specular)) * PointLight[i].Color;
		}

	if (UseDirLights)
		for (int i = 0; i < DirLightCount; i++)
		{
			LightComponents light = CalculateDirLight(DirLight[i]);
			float shadow = CalculateDirShadow(DirLight[i], 0.005);
			lighting += (light.Ambient + (1.0 - shadow) * (light.Diffuse + light.Specular)) * DirLight[i].Color;
		}

	if (UseSpotLights)
		for (int i = 0; i < SpotLightCount; i++)
		{
			LightComponents light = CalculateDirLight(SpotLight[i]);
			float shadow = CalculateDirShadow(SpotLight[i], 0.001);
			float circular = CalculateCircularShadow(SpotLight[i]);
			shadow = clamp(shadow + circular, 0, 1);
			lighting += (light.Ambient + (1.0 - shadow) * (light.Diffuse + light.Specular)) * SpotLight[i].Color;
		}

	lighting = clamp(lighting, 0, 1);
	lighting *= color;

	FragColor = vec4(lighting, 1.0f);
}