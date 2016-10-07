#include "Plane.h"

Plane::Plane(Texture& texture) : SimpleObject(glm::vec3(0,0,0),GetVertices(1), 4*8, GetIndizes(), 6, texture)
{
}

Plane::~Plane()
{
}

void Plane::Update(GLfloat deltaTime)
{
}

void Plane::Render(Shader& shader) const
{
	SimpleObject::Render(shader);
}

GLfloat* Plane::GetVertices(GLfloat size)
{
	GLfloat* vertices = new GLfloat[4*8] {
		// Positions          // Colors           // Texture Coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left 
	};
	return vertices;
}

GLuint* Plane::GetIndizes()
{
	GLuint* indices = new GLuint[6] {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};
	return indices;
}
