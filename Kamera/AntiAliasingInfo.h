#pragma once

struct AntiAliasingInfo
{
	GLsizei ColorSamples = 4;
	GLsizei CoverageSamples = 8;
	GLuint ColorBuffer;
	GLuint DepthBuffer;
};