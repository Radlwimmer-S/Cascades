#pragma once
#include <glm/detail/type_vec3.hpp>

struct RenderInfo
{
	float NormalMapFactor = 1.0f;
	bool EnableSoftShadowing = true;
	bool EnableLight = true;
	bool EnableShadows = true;
	bool DrawLightPosition = true;
	bool RenderPath = false;

	//Generator
	glm::ivec3 Resolution;
	int Seed;
	int StartLayer;
	float NoiseScale;
	glm::vec3 GeometryScale;
	float IsoLevel;
};
