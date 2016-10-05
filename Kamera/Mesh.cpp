#include "Mesh.h"
#include <glm/mat4x2.hpp>
#include <glm/gtc/type_ptr.hpp>


Mesh::Mesh(GLuint vao, GLuint indexCount): m_vao(vao), m_indexCount(indexCount)
{
}


Mesh::~Mesh()
{
}

void Mesh::Update(GLfloat delta_time)
{
}

void Mesh::Render(const Shader& shader)
{
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}
