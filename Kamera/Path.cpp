#include "Path.h"



Path::Path(glm::vec3* controlPoints, GLuint controlCount, GLuint duration) : m_controlPoints(controlPoints), m_controlCount(controlCount), m_duration(duration)
{
}


Path::~Path()
{
}
