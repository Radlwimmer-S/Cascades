#include "Model.h"
#include "Texture.h"
#include "Global.h"
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Vertex.h"
#include "Triangle.h"

Model::Model(glm::vec3 position, glm::quat orientaton, Triangle* tris, GLsizei triCount, glm::vec3 color, NormalBlendMode normalMode) : Model(position, orientaton, tris, triCount, color, ColorOnly, nullptr, normalMode, nullptr)
{}

Model::Model(glm::vec3 position, glm::quat orientaton, Triangle* tris, GLsizei triCount, glm::vec3 color, ColorBlendMode colorMode, Texture* texture, NormalBlendMode normalMode) : Model(position, orientaton, tris, triCount, color, colorMode, texture, normalMode, nullptr)
{}

Model::Model(glm::vec3 position, glm::quat orientaton, Triangle* tris, GLsizei triCount, glm::vec3 color, NormalBlendMode normalMode, Texture* normalMap) : Model(position, orientaton, tris, triCount, color, ColorOnly, nullptr, normalMode, normalMap)
{}

Model::Model(glm::vec3 position, glm::quat orientaton, Triangle* tris, GLsizei triCount, glm::vec3 color, ColorBlendMode colorMode, Texture* texture, NormalBlendMode normalMode, Texture* normalMap) : BaseObject(position, orientaton), triCount(triCount), m_color(color), m_colorMode(colorMode), m_normalMode(normalMode), m_texture(texture), m_normalMap(normalMap)
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, triCount * sizeof(Triangle), tris, GL_STATIC_DRAW);
	Vertex::ConfigVertexArrayObject(m_vao);
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
	GLint modelLoc = glGetUniformLocation(shader.Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(GetMatrix()));
	glCheckError();

	GLint colorModeLoc = glGetUniformLocation(shader.Program, "colorMode");
	glUniform1i(colorModeLoc, m_colorMode);
	glCheckError();

	GLint objectColorLoc = glGetUniformLocation(shader.Program, "objectColor");
	glUniform3fv(objectColorLoc, 1, glm::value_ptr(m_color));
	glCheckError();

	int textureLoc = glGetUniformLocation(shader.Program, "objectTexture");
	if (m_texture != nullptr)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_texture->GetId());
		glUniform1i(textureLoc, 1);
		glCheckError();
	}

	GLint normalModeLoc = glGetUniformLocation(shader.Program, "normalMode");
	glUniform1i(normalModeLoc, m_normalMode);
	glCheckError();

	GLint normalLoc = glGetUniformLocation(shader.Program, "normalMap");
	if (m_normalMap != nullptr)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_normalMap->GetId());
		glUniform1i(normalLoc, 2);
		glCheckError();
	}

	glBindVertexArray(m_vao);
	glCheckError();
	glDrawArrays(GL_TRIANGLES, 0, triCount * 3);

	glCheckError();

	glBindVertexArray(0);

	glUniform1i(textureLoc, 0);
	glCheckError();
	glUniform1i(normalLoc, 0);
	glCheckError();
}
