#version 330 core

out vec4 FragColor;

in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 UV;
	mat3 TBN;
} fs_in;

uniform bool SoftShadows = true;
uniform bool EnableLighting = true;

struct LightComponents
{
	float Ambient;
	float Diffuse;
	float Specular;
};

const int LIGHT_COUNT = 10;

struct LightSource
{
	vec3 Pos;
	vec3 Color;

	int Type;
	bool IsEnabled;

	mat4 lightSpaceMatrix;

	float near_plane;
	float far_plane;

	samplerCube depthCube;
	sampler2D depthMap;
};

uniform LightSource Lights[LIGHT_COUNT];

uniform int colorMode;
uniform vec3 objectColor;
uniform sampler2D objectTexture;

uniform int normalMode;
uniform sampler2D normalMap;
uniform float bumbiness = 2f;

uniform vec3 viewPos;

#pragma include "ShaderEnums.h"

vec3 DetermineFragmentColor(in int colorMode)
{
	//Check for blending colorMode
	switch (colorMode)
	{
		case COLOR_ONLY_MODE:
			return objectColor;
		case COLOR_MIX_MODE:
			return texture(objectTexture, fs_in.UV).rgb * objectColor;
		case TEXTURE_ONLY_MODE:
			return texture(objectTexture, fs_in.UV).rgb;
	}
}

vec3 DetermineFragmentNormal(in int normalMode)
{
	switch (normalMode)
	{
		case NORMALS_ONLY_MODE:
			return fs_in.Normal;
		case NO_NORMALS_MODE:
			return vec3(0.0f);
		case BUMP_MAP_ONLY_MODE:
			// Obtain normal from normal map in range [0,1]
			vec3 tmpNormal = texture(normalMap, fs_in.UV).rgb;
			// Transform normal vector to range [-1,1]
			tmpNormal = normalize(tmpNormal * 2.0 - 1.0);
			// Apply "Bumpiness"
			tmpNormal = normalize(tmpNormal * vec3(bumbiness, bumbiness, 1.0f));
			// Convert to global
			return normalize(fs_in.TBN * tmpNormal);
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

float CalculatePointShadow(in LightSource light, in vec3 normal)
{
	// Get vector between fragment position and light position
	vec3 fragToLight = fs_in.FragPos - light.Pos;
	// Get current linear depth as the length between the fragment and light position
	float currentDepth = length(fragToLight);
	float bias = 0.05;
	float shadow = 0.0;

	if (!SoftShadows)
	{
		// Use the light to fragment vector to sample from the depth map    
		float closestDepth = texture(light.depthCube, fragToLight).r;
		// It is currently in linear range between [0,1]. Re-transform back to original value
		closestDepth *= light.far_plane;
		shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	}
	else
	{
		// Test for shadows with PCF
		int samples = 20;
		float viewDistance = length(viewPos - fs_in.FragPos);
		float diskRadius = (1.0 + (viewDistance / light.far_plane)) / 25.0f;
		for (int i = 0; i < samples; ++i)
		{
			float closestDepth = texture(light.depthCube, fragToLight + gridSamplingDisk[i] * diskRadius).r;
			closestDepth *= light.far_plane;   // Undo mapping [0;1]

			float distance = length(light.Pos - fs_in.FragPos);
			float distanceBias = max(bias, bias * distance / 5.0f);
			if (currentDepth - distanceBias > closestDepth)
				shadow += 1.0;
		}
		shadow /= float(samples);
	}

	// return shadow;
	return shadow;
}

LightComponents CalculateLight(in LightSource light, in vec3 normal, in vec3 lightDir)
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
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), 64.0);
	lighting.Specular = specularStrength * spec;

	return lighting;
}

float CalculateDirShadow(in LightSource light, in vec3 normal, in float baseBias)
{
	vec4 fragPosLightSpace = light.lightSpaceMatrix * vec4(fs_in.FragPos, 1.0f);

	// perform perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// Transform to [0,1] range
	projCoords = projCoords * 0.5 + 0.5;

	// Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
	if (projCoords.z > 1.0)
		return 0.0;

	// Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
	float closestDepth = texture(light.depthMap, projCoords.xy).r;

	// Get depth of current fragment from light's perspective
	float currentDepth = projCoords.z;
	// Calculate bias (based on depth map resolution and slope)
	vec3 lightDir = normalize(light.Pos);

	float bias = max(baseBias / 10 * (1.0 - dot(normal, lightDir)), baseBias);

	float shadow = 0.0;
	// Check whether current frag pos is in shadow
	if (!SoftShadows)
		shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	else
	{
		// PCF	
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
	}

	return shadow;
}

float CalculateCircularShadow(in LightSource light, in vec3 normal)
{
	vec4 fragPosLightSpace = light.lightSpaceMatrix * vec4(fs_in.FragPos, 1.0f);

	// perform perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

	//Generate Circular Shadow
	float distanceToCenter = length(projCoords.xy);
	return clamp(distanceToCenter * distanceToCenter, 0, 1);
}

float CalculateDistanceShadow(in LightSource light, in vec3 normal)
{
	float distance = length(light.Pos - fs_in.FragPos);
	return distance / light.far_plane;
}

vec3 CalculateDirLightSource(in LightSource light, in vec3 normal)
{
	LightComponents components = CalculateLight(light, normal, normalize(light.Pos));
	float shadow = CalculateDirShadow(light, normal, 0.005);
	return (components.Ambient + (1.0 - shadow) * (components.Diffuse + components.Specular)) * light.Color;
}

vec3 CalculateSpotLightSource(in LightSource light, in vec3 normal)
{
	LightComponents components = CalculateLight(light, normal, normalize(light.Pos - fs_in.FragPos));
	float shadow = CalculateDirShadow(light, normal, 0.002);
	float circular = CalculateCircularShadow(light, normal);
	shadow = clamp(shadow + circular, 0, 1);
	return (components.Ambient + (1.0 - shadow) * (components.Diffuse + components.Specular)) * light.Color;
}

vec3 CalculatePointLightSource(in LightSource light, in vec3 normal)
{
	LightComponents components = CalculateLight(light, normal, normalize(light.Pos - fs_in.FragPos));
	float shadow = CalculatePointShadow(light, normal);
	float distance = CalculateDistanceShadow(light, normal);
	shadow = clamp(shadow + distance, 0, 1);
	return (components.Ambient + (1.0 - shadow) * (components.Diffuse + components.Specular)) * light.Color;
}


void main()
{
	vec3 color = DetermineFragmentColor(colorMode);

	vec3 normal = DetermineFragmentNormal(normalMode);

	if (!EnableLighting || normal == vec3(0.0f))
	{
		FragColor = vec4(color, 1.0f);
		return;
	}	

	vec3 lighting = vec3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < LIGHT_COUNT; i++)
	{
		if (!Lights[i].IsEnabled)
			continue;

		switch (Lights[i].Type)
		{
		case DIR_LIGHT:
			lighting += CalculateDirLightSource(Lights[i], normal);
			break;
		case SPOT_LIGHT:
			lighting += CalculateSpotLightSource(Lights[i], normal);
			break;
		case POINT_LIGHT:
			lighting += CalculatePointLightSource(Lights[i], normal);
			break;
		}
	}

	lighting = clamp(lighting, 0, 1);
	lighting *= color;

	FragColor = vec4(lighting, 1.0f);
}