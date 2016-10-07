#include "Box.h"
#include "Texture.h"


Box::Box(glm::vec3  position, glm::vec3 size, Texture& texture) : Model(position, GetVertices(size), 8, GetIndizes(), 12)
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
	Model::Render(shader);
}

GLfloat* Box::GetVertices(glm::vec3 size)
{
	GLfloat* vertices = new GLfloat[3 * 8]{
		 -1.0f * size.x, -1.0f * size.y,  1.0f * size.z,
		  1.0f * size.x, -1.0f * size.y,  1.0f * size.z,
		 -1.0f * size.x,  1.0f * size.y,  1.0f * size.z,
		  1.0f * size.x,  1.0f * size.y,  1.0f * size.z,
		 -1.0f * size.x,  1.0f * size.y, -1.0f * size.z,
		  1.0f * size.x,  1.0f * size.y, -1.0f * size.z,
		 -1.0f * size.x, -1.0f * size.y, -1.0f * size.z,
		  1.0f * size.x, -1.0f * size.y, -1.0f * size.z
	};
	return vertices;
}

GLuint* Box::GetIndizes()
{
	GLuint* indices = new GLuint[12 * 3]{  // Note that we start from 0!
		0, 1, 2, // Front 1
		2, 1, 3, // Front 2
		2, 3, 4, // Left 1
		4, 3, 5, // Left 2
		4, 5, 6, // Bottom 1
		6, 5, 7, // Bottom 2
		6, 7, 0, // Back 1
		0, 7, 1, // Back 2
		1, 7, 3, // Right 1
		3, 7, 5, // Right 2
		6, 0, 4, // Top 1
		4, 0, 2  // Top 2
	};
	return indices;
}
