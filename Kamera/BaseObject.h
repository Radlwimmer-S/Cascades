#pragma once
#include <glm/detail/type_vec3.hpp>
#include <GL/glew.h>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Shader;

class BaseObject
{
public:
	BaseObject(glm::vec3 position, glm::quat orientaton);
	virtual ~BaseObject();
	virtual void Update(GLfloat deltaTime) = 0;
	virtual void Render(Shader& shader) const = 0;

	GLuint GetVAO() const
	{
		return m_vao;
	}

	glm::mat4 GetMatrix() const
	{
		glm::mat4 translation = glm::mat4(1.0f);
		translation = glm::translate(translation, m_position);
		glm::mat4 rotation = glm::toMat4(glm::normalize(m_orientation));

		return translation *  rotation;
	}

protected:
	glm::vec3 m_position;
	glm::quat m_orientation;
	GLuint m_vao;
};

