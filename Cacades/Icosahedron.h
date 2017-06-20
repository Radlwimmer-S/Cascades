#pragma once
#include "BaseObject.h"
class Icosahedron :
	public BaseObject
{
public:
	Icosahedron(glm::vec3 position, glm::quat orientaton, glm::vec3 color);
	virtual ~Icosahedron();

	void Update(GLfloat deltaTime) override;
	void Render(Shader& shader) const override;

	void SetLightingMode(bool active);
	void SetColor(glm::vec3 color);
protected:

	GLuint m_vbo, m_vio;
	GLuint m_vao;
	glm::vec3 m_color;
	GLuint m_indexCount;
	bool m_lightingMode;

	const int Faces[20 * 3] = {
		2, 1, 0,
		3, 2, 0,
		4, 3, 0,
		5, 4, 0,
		1, 5, 0,
		11, 6,  7,
		11, 7,  8,
		11, 8,  9,
		11, 9,  10,
		11, 10, 6,
		1, 2, 6,
		2, 3, 7,
		3, 4, 8,
		4, 5, 9,
		5, 1, 10,
		2,  7, 6,
		3,  8, 7,
		4,  9, 8,
		5, 10, 9,
		1, 6, 10 };

	const float Verts[12 * 3] = {
		0.000f,  0.000f,  1.000f,
		0.894f,  0.000f,  0.447f,
		0.276f,  0.851f,  0.447f,
		-0.724f,  0.526f,  0.447f,
		-0.724f, -0.526f,  0.447f,
		0.276f, -0.851f,  0.447f,
		0.724f,  0.526f, -0.447f,
		-0.276f,  0.851f, -0.447f,
		-0.894f,  0.000f, -0.447f,
		-0.276f, -0.851f, -0.447f,
		0.724f, -0.526f, -0.447f,
		0.000f,  0.000f, -1.000f };
};

