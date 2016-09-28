#pragma once
#include "Object.h"
class Plane :
	public Object
{
public:
	Plane(Texture& texture);
	~Plane();

	void Update(GLfloat deltaTime) override;
	void Render(Shader& shader) const override;
private:
	static GLfloat* GetVertices(GLfloat size);
	static GLuint* GetIndizes();
};

