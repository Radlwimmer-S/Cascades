#pragma once
#include <GL/glew.h>
#include "Texture.h"
#include "Enums.h"
#include <glm/detail/type_vec3.hpp>

class TriplanarMesh
{
public:
	TriplanarMesh();
	~TriplanarMesh();

private:
	GLuint m_vbo;
	GLuint m_vao;
	GLsizei m_triCount;
	glm::vec3 m_color;
	ColorBlendMode m_colorMode;
	NormalBlendMode m_normalMode;
	Texture* m_texture;
	Texture* m_normalMap;
	Texture* m_displacementMap;
};

