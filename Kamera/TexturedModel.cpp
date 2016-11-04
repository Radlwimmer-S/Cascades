#include "TexturedModel.h"
#include "Texture.h"
#include "Global.h"
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

TexturedModel::TexturedModel(glm::vec3 position, glm::quat orientaton, GLfloat* vertices, GLsizei vertexCount, VertexFormat vertexFormat, glm::vec3 color, Texture& texture) : Model(position, orientaton, vertices, vertexCount, vertexFormat, color), m_Texture(texture)
{
	m_colorMode = TextureOnly;
}

TexturedModel::~TexturedModel()
{
	Model::~Model();
	glDeleteTextures(1, new GLuint[1]{ m_Texture.GetId() });
}

void TexturedModel::Render(Shader& shader) const
{
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_Texture.GetId());
	glUniform1i(glGetUniformLocation(shader.Program, "objectTexture"), 1);
	glCheckError();

	Model::Render(shader);
}
