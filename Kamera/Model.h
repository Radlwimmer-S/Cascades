#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader;
class Texture;

class Model
{
public:
	Model(glm::vec3 position, GLfloat* vertices, GLsizei vertexCount, glm::vec3 color);
	virtual ~Model();

	virtual void Update(GLfloat deltaTime);
	virtual void Render(Shader& shader) const;

	GLuint GetVAO() const
	{
		return m_vao;
	}

private:
	GLuint m_vbo, m_vao;
	GLsizei m_vertexCount;
	glm::vec3 m_position;
	glm::vec3 m_color;
};

