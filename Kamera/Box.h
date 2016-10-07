#pragma once
#include "Model.h"

class Box : public Model
{
public:
	Box(glm::vec3 position, glm::vec3 size, Texture& texture);
	~Box();

	void Update(GLfloat deltaTime) override;
    void Render(Shader& shader) const override;
private:
	static GLfloat* GetVertices(glm::vec3 size);
	static GLuint* GetIndizes();
};

