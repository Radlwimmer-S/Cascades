#pragma once

enum State
{
	Loading,
	Running,
	Paused,
	Stopped,
};

enum ColorBlendMode
{
	ColorOnly = -1,
	Mix = 0,
	TextureOnly = 1,
};

enum LightType
{
	Directional, Point
};

enum VertexFormat
{
	V = 3,
	VN = 6,
	VNT = 8,
};