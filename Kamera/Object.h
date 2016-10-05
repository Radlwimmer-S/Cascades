#pragma once

#include <GL/glew.h>
#include "Mesh.h"
#include "Box.h"
#include <vector>

class Object
{
public:
	Object(std::string filePath, glm::vec3 position);
	~Object();
	void Update(GLfloat delta_time);
	void Render(const Shader& shader);
private:
	GLuint m_vbo;
	std::vector<Mesh*> m_meshes;
	glm::vec3 m_position;
};

