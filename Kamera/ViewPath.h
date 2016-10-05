#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Path.h"

class ViewPath : public Path
{
public:
	ViewPath(glm::vec3* controlPoints, GLuint controlCount, GLuint duration);
	~ViewPath();
	glm::quat GetRotation();
};

