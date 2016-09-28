#include "CameraPath.h"

CameraPath::CameraPath(glm::vec3* controlPoints, GLuint controlCount, GLuint duration) : Path(controlPoints, controlCount, duration)
{
}

CameraPath::~CameraPath()
{
}
