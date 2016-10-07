#pragma once
#include <glm/detail/type_vec3.hpp>
#include <GL/glew.h>

class Shader;

class BaseObject
{
public:
	BaseObject(glm::vec3 position);
	virtual ~BaseObject();

	virtual void Update(GLfloat deltaTime) = 0;
	virtual void Render(Shader& shader) const = 0;

	glm::vec3 m_position;
	GLuint m_vao;
};

