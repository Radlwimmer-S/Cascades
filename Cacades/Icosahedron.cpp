#include "Icosahedron.h"
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

Icosahedron::Icosahedron(glm::vec3 position, glm::quat orientaton, glm::vec3 color): BaseObject(position, orientaton), m_vbo(0), m_vio(0), m_vao(0), m_color(color), m_indexCount(sizeof(Faces) / sizeof(Faces[0]))
{
	// Create the VAO:
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// Create the VBO for positions:
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Verts), Verts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(VS_IN_POSITION);
	glVertexAttribPointer(VS_IN_POSITION, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	// Create the VBO for indices:
	glGenBuffers(1, &m_vio);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vio);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Faces), Faces, GL_STATIC_DRAW);
}

void Icosahedron::Update(GLfloat deltaTime)
{
	BaseObject::Update(deltaTime);
}

void Icosahedron::Render(Shader& shader) const
{
	GLint modelLoc = glGetUniformLocation(shader.Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(GetMatrix()));
	glCheckError();

	GLint objectColorLoc = glGetUniformLocation(shader.Program, "objectColor");
	glUniform3fv(objectColorLoc, 1, glm::value_ptr(m_color));
	glCheckError();

	GLint lightingLoc = glGetUniformLocation(shader.Program, "EnableLighting");
	glUniform1i(lightingLoc, m_lightingMode);
	glCheckError();

	glBindVertexArray(m_vao);
	glCheckError(); 
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	glDrawElements(GL_PATCHES, m_indexCount, GL_UNSIGNED_INT, 0);

	glCheckError();

	glBindVertexArray(0);
}

void Icosahedron::SetLightingMode(bool isActive)
{
	m_lightingMode = isActive;
}

void Icosahedron::SetColor(glm::vec3 color)
{
	m_color = color;
}

Icosahedron::~Icosahedron()
{
}
