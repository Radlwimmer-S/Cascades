#pragma once
#include <sstream>
#include <GL/glew.h>

struct AntiAliasingInfo
{
	GLsizei ColorSamples = 4;
	GLsizei ActiveColorSamples = 4;
	GLsizei MaxColorSamples;
	GLsizei CoverageSamples = 8;
	GLsizei ActiveCoverageSamples = 8;
	GLsizei MaxCoverageSamples;
	GLuint ColorBuffer;
	GLuint DepthBuffer;
	GLuint Quality = GL_NICEST;

	void Init()
	{
		glGenRenderbuffersEXT(1, &ColorBuffer);
		glGenRenderbuffersEXT(1, &DepthBuffer);


		if (GLEW_NV_framebuffer_multisample_coverage)
		{
			int maxModes;
			glGetIntegerv(GL_MAX_MULTISAMPLE_COVERAGE_MODES_NV, &maxModes);
			if (maxModes != 0)
			{
				int* modes = new int[2 * maxModes];
				glGetIntegerv(GL_MULTISAMPLE_COVERAGE_MODES_NV, modes);

				MaxCoverageSamples = modes[2 * (maxModes - 1)];
				MaxColorSamples = modes[2 * (maxModes - 1) + 1];
				return;
			}
		}

		MaxCoverageSamples = 16;
		MaxColorSamples = 16;
	}

	std::string ParseAAMode() const
	{
		if (CoverageSamples == 0)
			return "Deaktiviert";

		std::stringstream ss;

		if ((ActiveCoverageSamples == 8 || ActiveCoverageSamples == 16) && (ActiveColorSamples == 4 || ActiveColorSamples == 8))
		{
			ss << ActiveCoverageSamples;
			if (ActiveColorSamples == 8)
				ss << "xQ (Quality) CSAA";
			else
				ss << "x CSAA";

			return ss.str();
		}

		if (ActiveColorSamples == 0 || ActiveCoverageSamples >= ActiveColorSamples)
		{
			ss << ActiveCoverageSamples << "x MSAA";
			return ss.str();
		}

		ss << "Custom CSAA";
		return ss.str();
	}
};
