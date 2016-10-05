#pragma once
#include "Object_old.h"

class Box : public Object_old
{
public:
	Box(glm::vec3 position, GLfloat size, Texture& texture);
	~Box();

	void Update(GLfloat deltaTime) override;
    void Render(Shader& shader) const override;
private:
	static GLfloat* GetVertices(GLfloat size);
	static GLuint* GetIndizes();
};

