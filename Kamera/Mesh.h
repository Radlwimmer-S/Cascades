#pragma once
#include <GL/glew.h>
#include <string>

class Shader;

class Mesh
{
public:
	Mesh(GLuint vao, GLuint indexCount);
	~Mesh();
	void Update(GLfloat delta_time);
	void Render(const Shader& shader);

private:
	std::string name;
	GLuint m_vao, m_indexCount;
};

