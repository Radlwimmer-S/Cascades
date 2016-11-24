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
	int textureLoc = glGetUniformLocation(shader.Program, "objectTexture");
	glUniform1i(textureLoc, 1);
	glCheckError();

	GLint normalLoc = glGetUniformLocation(shader.Program, "normalMap");
	if (m_normalMap != nullptr)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_normalMap->GetId());
		glUniform1i(normalLoc, 2);
		glCheckError();
	}

	Model::Render(shader);
	
	glUniform1i(textureLoc, 0);
	glCheckError();
	glUniform1i(normalLoc, 0);
	glCheckError();
}
