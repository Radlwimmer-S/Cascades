#pragma once

#include "shaders/EnumNormalMode.h"
#include "shaders/EnumColorMode.h"
#include "shaders/EnumLightType.h"
#include "shaders/EnumParticleType.h"

enum NormalBlendMode
{
	NormalsOnly = NORMALS_ONLY_MODE,
	NoNormals = NO_NORMALS_MODE,
	BumpMapOnly = BUMP_MAP_ONLY_MODE,
};

enum ColorBlendMode
{
	ColorOnly = COLOR_ONLY_MODE,
	Mix = COLOR_MIX_MODE,
	TextureOnly = TEXTURE_ONLY_MODE,
};

enum ParticleType
{
	EmitterRay = PARTICLE_RAY,
	Emitter = PARTICLE_EMITTER,
	FlowingWater = PARTICLE_WATER_FLOWING,
	FallingWater = PARTICLE_WATER_FALLING,
	CollisionMist = PARTICLE_MIST_COLLISION,
	FallingMist = PARTICLE_MIST_FALLING,
};

enum State
{
	Loading,
	Running,
	Paused,
	Stopped,
};

enum LightType
{
	Directional = DIR_LIGHT,
	Spot = SPOT_LIGHT,
	Point = POINT_LIGHT,
};

enum VertexFormat
{
	V = 3,
	VN = 6,
	VNT = 8,
};