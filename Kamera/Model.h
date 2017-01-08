#pragma once
#include "BaseObject.h"
#include "Enums.h"

class Texture;
struct Triangle;

class Model : public BaseObject
{
public:
	Model(glm::vec3 position, glm::quat orientaton, Triangle* tris, GLsizei triCount, glm::vec3 color, NormalBlendMode normalMode = NormalsOnly);
	Model(glm::vec3 position, glm::quat orientaton, Triangle* tris, GLsizei triCount, glm::vec3 color, ColorBlendMode colorMode, Texture* texture, NormalBlendMode normalMode = NormalsOnly);
	Model(glm::vec3 position, glm::quat orientaton, Triangle* tris, GLsizei triCount, glm::vec3 color, NormalBlendMode normalMode, Texture* normalMap);
	Model(glm::vec3 position, glm::quat orientaton, Triangle* tris, GLsizei triCount, glm::vec3 color, ColorBlendMode colorMode, Texture* texture, NormalBlendMode normalMode, Texture* normalMap);
	virtual ~Model();

	void Update(GLfloat deltaTime) override;
	void Render(Shader& shader) const override;

	GLuint GetVAO() const
	{
		return m_vao;
	}
protected:
	GLuint m_vbo;
	GLsizei triCount;
	glm::vec3 m_color;
	ColorBlendMode m_colorMode;
	NormalBlendMode m_normalMode;
	GLuint m_vao;
	Texture* m_texture;
	Texture* m_normalMap;
};

