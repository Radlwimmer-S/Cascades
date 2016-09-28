#include "Box.h"
#include "Texture.h"


Box::Box(glm::vec3  position, GLfloat size, Texture& texture) : Object(position, GetVertices(size), 8*8, GetIndizes(), 12*3, texture)
{
}


Box::~Box()
{
}

void Box::Update(GLfloat deltaTime)
{
}

void Box::Render(Shader& shader) const
{
	Object::Render(shader);
}

GLfloat* Box::GetVertices(GLfloat size)
{
	GLfloat* vertices = new GLfloat[8 * 8]{
		// Positions          // Colors           // Texture Coords
		 0.5f*size,  0.5f*size,  0.5f*size,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Front Top    Right
		 0.5f*size, -0.5f*size,  0.5f*size,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Front Bottom Right
		-0.5f*size, -0.5f*size,  0.5f*size,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Front Bottom Left
		-0.5f*size,  0.5f*size,  0.5f*size,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f, // Front Top    Left 
		 0.5f*size,  0.5f*size, -0.5f*size,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Back  Top    Right
		 0.5f*size, -0.5f*size, -0.5f*size,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Back  Bottom Right
		-0.5f*size, -0.5f*size, -0.5f*size,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Back  Bottom Left
		-0.5f*size,  0.5f*size, -0.5f*size,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Back  Top    Left 
	};
	return vertices;
}

GLuint* Box::GetIndizes()
{
	GLuint* indices = new GLuint[12 * 3]{  // Note that we start from 0!
		0, 1, 3, // Front 1
		1, 2, 3, // Front 2
		2, 3, 7, // Left 1
		3, 6, 7, // Left 2
		1, 2, 6, // Bottom 1
		2, 5, 6, // Bottom 2
		4, 5, 7, // Back 1
		5, 6, 7, // Back 2
		0, 1, 5, // Right 1
		1, 4, 5, // Right 2
		0, 3, 7, // Top 1
		3, 4, 7  // Top 2
	};
	return indices;
}
