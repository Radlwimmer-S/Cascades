#include "TexturedModel.h"
#include "Texture.h"
#include "Global.h"
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

TexturedModel::TexturedModel(glm::vec3 position, glm::quat orientaton, Triangle* tris, GLsizei triCount, glm::vec3 color, Texture& texture, Texture* normalMap) : Model(position, orientaton, tris, triCount, color), m_Texture(texture), m_normalMap(normalMap)
{
	m_colorMode = TextureOnly;
}

TexturedModel::~TexturedModel()
{
	Model::~Model();
	//glDeleteTextures(1, new GLuint[1]{ m_Texture.GetId() });
}

void TexturedModel::Render(Shader& shader) const
{
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_Texture.GetId());
	glUniform1i(glGetUniformLocation(shader.Program, "objectTexture"), 1);
	glCheckError();

	if (m_normalMap != nullptr)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_normalMap->GetId());
		glUniform1i(glGetUniformLocation(shader.Program, "normalMap"), 2);
		glCheckError();
	}

	Model::Render(shader);
	
	glUniform1i(glGetUniformLocation(shader.Program, "objectTexture"), 0);
	glUniform1i(glGetUniformLocation(shader.Program, "normalMap"), 0);
}
