#include "Model.h"
#include "Texture.h"
#include "Shader.h"
#include "Global.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.inl>

Model::Model(glm::vec3 position, GLfloat* vertices, GLsizei vertexCount, GLuint* indices, GLsizei indexCount) : m_texture(nullptr), m_vertexCount(vertexCount), m_indexCount(indexCount), m_position(position)
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * m_vertexCount, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 3 * m_indexCount, indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(SHADER_POSITION, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(SHADER_POSITION);

	glBindVertexArray(0); // Unbind m_vao
}

Model::Model(glm::vec3 position, GLfloat* vertices, GLsizei vertexCount, GLuint* indices, GLsizei indexCount, Texture& texture) : m_texture(&texture), m_vertexCount(vertexCount), m_indexCount(indexCount), m_position(position)
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * m_vertexCount, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 3 * m_indexCount, indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(SHADER_POSITION, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(SHADER_POSITION);
	// Color attribute
	glVertexAttribPointer(SHADER_COLOR, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(SHADER_COLOR);
	// TexCoord attribute
	glVertexAttribPointer(SHADER_TEXTURE, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(SHADER_TEXTURE);

	glBindVertexArray(0); // Unbind m_vao
}

Model::~Model()
{
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
}

void Model::Update(GLfloat deltaTime)
{
}

void Model::Render(Shader& shader) const
{
	//glTranslatef(m_position[0], m_position[1], m_position[2]);

	if (m_texture != nullptr)
	{
		// Bind Textures using texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture->GetId());
		glUniform1i(glGetUniformLocation(shader.Program, "ourTexture"), 0);
	}

	glm::mat4 modelPos;
	modelPos = glm::translate(modelPos, m_position);
	GLuint modelLocation = glGetUniformLocation(shader.Program, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelPos));

	// Render container
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}
