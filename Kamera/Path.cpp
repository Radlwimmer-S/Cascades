#include "Path.h"

Path::Path(glm::vec3* controlPoints, GLuint controlCount, GLuint duration) : m_controlPoints(controlPoints), m_controlCount(controlCount), m_duration(duration), m_timer(0.0f), m_pathLength(0.0f)
{
	m_controlVectors = new glm::vec3[m_controlCount - 1];
	for (int i = 0; i < m_controlCount - 1; i++)
	{
		// V0 = P1 - P0
		m_controlVectors[i] = m_controlPoints[i + 1] - m_controlPoints[i];
		m_pathLength += m_controlVectors[i].length();
	}

	m_velocity = m_pathLength / m_duration;

	m_controlTimes = new GLfloat[m_controlCount];
	m_controlTimes[0] = 0.0f;
	m_controlTimes[m_controlCount - 1] = m_duration;
	for (int i = 1; i < m_controlCount; i++)
	{
		// t1 = t0 + (s0 / v)
		m_controlTimes[i] = m_controlTimes[i - 1] + (m_controlVectors[i - 1].length() / m_velocity);
	}
}


Path::~Path()
{
}
