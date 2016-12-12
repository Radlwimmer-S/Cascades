#pragma once
#include "Engine.h"

struct RenderInfo
{
	float NormalMapFactor = 1.0f;
	bool EnableSoftShadowing = true;
	bool EnableLight = true;
	bool EnableShadows = true;
	bool DrawLightPosition = true;
};
