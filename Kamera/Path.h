#pragma once
#include <glm/detail/type_vec3.hpp>
#include <GL/glew.h>

class Path
{
public:
	Path(glm::vec3* controlPoints, GLuint controlCount, GLuint duration);
	~Path();

private:
	glm::vec3* m_controlPoints;
	GLuint m_controlCount;
	GLuint m_duration;
};

