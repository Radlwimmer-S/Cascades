#pragma once
#include "Object_old.h"
class Plane :
	public Object_old
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

