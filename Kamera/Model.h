#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "BaseObject.h"
#include "Enums.h"

class Shader;
class Texture;

class Model : public BaseObject
{
public:
	Model(glm::vec3 position, glm::quat orientaton, GLfloat* vertices, GLsizei vertexCount, VertexFormat vertexFormat, glm::vec3 color);
	virtual ~Model();

	void Update(GLfloat deltaTime) override;
	void Render(Shader& shader) const override;

	GLuint GetVAO() const
	{
		return m_vao;
	}
protected:
	GLuint m_vbo;
	GLsizei m_vertexCount;
	glm::vec3 m_color;
	ColorBlendMode m_colorMode;
	GLuint m_vao;
};

