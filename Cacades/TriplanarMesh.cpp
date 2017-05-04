#include "TriplanarMesh.h"
#include "Global.h"


TriplanarMesh::TriplanarMesh()
{
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_triCount * 3 * sizeof(glm::vec3) * 2, nullptr, GL_STATIC_DRAW);
	glCheckError();
	// Position attribute
	glEnableVertexAttribArray(VS_IN_POSITION);
	glVertexAttribPointer(VS_IN_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 2, (GLvoid*)0);
	glEnableVertexAttribArray(VS_IN_NORMAL);
	glVertexAttribPointer(VS_IN_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 2, (GLvoid*)sizeof(glm::vec3));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glCheckError();
	glBindVertexArray(0);
}


TriplanarMesh::~TriplanarMesh()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
}

GLuint TriplanarMesh::GetVAO() const
{
	return m_vao;
}

GLuint TriplanarMesh::GetVBO() const
{
	return m_vbo;
}

void TriplanarMesh::UpdateVao(int triCount)
{
	m_triCount = triCount;
}

GLsizei TriplanarMesh::GetTriCount() const
{
	return m_triCount;
}
