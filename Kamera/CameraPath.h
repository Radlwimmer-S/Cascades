#pragma once
#include "Path.h"

class CameraPath : public Path
{
public:
	CameraPath(glm::vec3* controlPoints, GLuint controlCount, GLuint duration);
	~CameraPath();
};

