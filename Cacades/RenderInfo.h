#pragma once
#include <glm/detail/type_vec3.hpp>
#include "Model.h"

struct RenderInfo
{
	float NormalMapFactor = 1.0f;
	ShadowMode ShadowMode = PcfShadows;
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
	bool WireFrameMode;

	int DisplacementInitialSteps = 16;
	int DisplacementRefinementSteps = 16;
	float DisplacementScale = 0.025f;
};
