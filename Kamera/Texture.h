#pragma once
#include <GL/glew.h>

class Texture
{
public:
	Texture(GLchar* texturePath, GLchar* name);
	~Texture();

	GLuint GetId() const;
	GLchar* GetName() const;

private:
	GLuint m_id;
	GLchar* m_name;
};

