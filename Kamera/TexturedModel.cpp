#include "TexturedModel.h"
#include "Texture.h"
#include "Global.h"
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

TexturedModel::TexturedModel(glm::vec3 position, GLfloat* vertices, GLsizei vertexCount, glm::vec3 color, Texture& texture) : Model(position, vertexCount, color), m_Texture(texture)
{
	const int VertexSize = 8 * sizeof(GLfloat);

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * VertexSize, vertices, GL_STATIC_DRAW);

	glBindVertexArray(m_vao);
	// Position attribute
	glVertexAttribPointer(SHADER_POSITION, 3, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)0);
	glEnableVertexAttribArray(SHADER_POSITION);
	// Normal attribute
	glVertexAttribPointer(SHADER_NORMAL, 3, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(SHADER_NORMAL);
	// Texture attribute
	glVertexAttribPointer(SHADER_TEXTURE, 2, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(SHADER_TEXTURE);

	glBindVertexArray(0);
}

TexturedModel::~TexturedModel()
{
	Model::~Model();
	glDeleteTextures(1, new GLuint[1]{ m_Texture.GetId() });
}

void TexturedModel::Render(Shader& shader) const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Texture.GetId());
	glUniform1i(glGetUniformLocation(shader.Program, "objectTexture"), 0);

	Model::Render(shader);
}
