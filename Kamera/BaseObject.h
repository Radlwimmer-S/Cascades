#pragma once
#include <glm/detail/type_vec3.hpp>
#include <GL/glew.h>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define glCheckError() glCheckError_(__FILE__, __LINE__) 

class Shader;

class BaseObject
{
public:
	BaseObject(glm::vec3 position);
	virtual ~BaseObject();
	virtual void Update(GLfloat deltaTime) = 0;
	virtual void Render(Shader& shader) const = 0;

	GLuint GetVAO() const
	{
		return m_vao;
	}

	glm::mat4 GetMatrix() const
	{
		glm::mat4 model = glm::toMat4(glm::normalize(m_orientation));;
		model = glm::translate(model, m_position);
		return model;
	}

protected:
	GLenum glCheckError_(const char* file, int line) const;

	glm::vec3 m_position;
	glm::quat m_orientation;
	GLuint m_vao;
};

