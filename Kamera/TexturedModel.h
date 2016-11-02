#pragma once
#include "Model.h"
class TexturedModel : public Model
{
public:
	TexturedModel(glm::vec3 position, GLfloat* vertices, GLsizei vertexCount, glm::vec3 color, Texture& texture);
	TexturedModel(glm::vec3 position, glm::quat rotation, GLfloat* vertices, GLsizei vertexCount, glm::vec3 color, Texture& texture);
	virtual ~TexturedModel();

	void Render(Shader& shader) const override;

private:
	Texture& m_Texture;
};

