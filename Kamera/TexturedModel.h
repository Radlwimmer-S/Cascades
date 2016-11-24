#pragma once
#include "Model.h"
class TexturedModel : public Model
{
public:
	TexturedModel(glm::vec3 position, glm::quat orientaton, Triangle* tris, GLsizei triCount, glm::vec3 color, Texture& texture, Texture* normalMap = nullptr);
	virtual ~TexturedModel();

	void Render(Shader& shader) const override;

private:
	Texture& m_Texture;
	Texture* m_normalMap;
};

