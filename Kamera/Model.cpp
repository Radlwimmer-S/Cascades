#include "Model.h"
#include "Texture.h"
#include "Shader.h"
#include "Global.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.inl>

Model::Model(glm::vec3 position, GLfloat* vertices, GLsizei vertexCount, glm::vec3 color) : m_vertexCount(vertexCount), m_position(position), m_color(color)
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glBindVertexArray(m_vao);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}

Model::~Model()
{
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
}

void Model::Update(GLfloat deltaTime)
{
}

void Model::Render(Shader& shader) const
{
	GLint objectColorLoc = glGetUniformLocation(shader.Program, "objectColor");
	glUniform3f(objectColorLoc, m_color.x, m_color.y, m_color.z);

	glm::mat4 model;
	model = glm::translate(model, m_position);
	GLint modelLoc = glGetUniformLocation(shader.Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
