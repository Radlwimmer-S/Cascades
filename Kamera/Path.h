#pragma once
#include <glm/detail/type_vec3.hpp>
#include <GL/glew.h>

class Shader;

class Path
{
public:
	Path(glm::vec3* controlPoints, GLuint controlCount, GLuint duration);
	virtual ~Path();

	virtual void Update(GLfloat deltaTime) = 0;
	virtual void Render(Shader& shader) const = 0;

	glm::vec3 GetPosition() const
	{
		return m_position;
	}

private:
	glm::vec3* m_controlPoints;
	GLuint m_controlCount;
	glm::vec3* m_controlVectors;
	GLfloat* m_controlTimes;
	GLuint m_duration;
	GLfloat m_timer;
	GLfloat m_pathLength;
	GLfloat m_velocity;
	glm::vec3 m_position;
};

