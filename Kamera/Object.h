#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader;
class Texture;

class Object
{
public:
	Object(glm::vec3 position, GLfloat* vertices, GLsizei vertexCount, GLuint* indices, GLsizei indexCount, Texture& texture);
	virtual ~Object();

	virtual void Update(GLfloat deltaTime);
	virtual void Render(Shader& shader) const;

	GLuint GetVAO() const
	{
		return m_vao;
	}

private:
	GLuint m_vbo, m_vao, m_ebo;
	Texture& m_texture;
	GLsizei m_vertexCount;
	GLsizei m_indexCount;
	glm::vec3 m_position;
};

