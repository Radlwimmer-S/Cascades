#include "ViewPath.h"
#include "glm/gtx/quaternion.hpp"


ViewPath::ViewPath(glm::vec3* controlPoints, GLuint controlCount, GLuint duration) : Path(controlPoints, controlCount, duration)
{
}


ViewPath::~ViewPath()
{
}

glm::quat ViewPath::GetRotation()
{
	return glm::quat(0, 0, 0, 0);
}
