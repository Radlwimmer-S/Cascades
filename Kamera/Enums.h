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