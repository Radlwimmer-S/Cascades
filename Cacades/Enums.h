#pragma once

#include "shaders/ShaderEnums.h"

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