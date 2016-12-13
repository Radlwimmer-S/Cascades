#pragma once
#include <sstream>
#include <GL/glew.h>

struct AntiAliasingInfo
{
	GLsizei ColorSamples = 4;
	GLsizei CoverageSamples = 8;
	GLuint ColorBuffer;
	GLuint DepthBuffer;

	std::string ParseAAMode() const
	{
		if (CoverageSamples == 0)
			return "Deaktiviert";

		std::stringstream ss;

		if ((CoverageSamples == 8 || CoverageSamples == 16) && (ColorSamples == 4 || ColorSamples == 8))
		{
			ss << CoverageSamples;
			if (ColorSamples == 8)
				ss << "xQ (Quality) CSAA";
			else
				ss << "x CSAA";

			return ss.str();
		}

		if (ColorSamples == 0 || CoverageSamples >= ColorSamples)
		{
			ss << CoverageSamples << "x MSAA";
			return ss.str();
		}

		ss << "Custom CSAA";
		return ss.str();
	}
};
