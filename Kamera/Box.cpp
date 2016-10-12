#include "Box.h"
#include "Texture.h"


Box::Box(glm::vec3 position, glm::vec3 size, glm::vec3 color) : Model(position, GetVertices(size), 36 * 6, color)
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
	GLfloat* vertices = new GLfloat[36 * 6]{
		-0.5f * size.x, -0.5f * size.y, -0.5f * size.z,  0.0f,  0.0f, -1.0f,
		+0.5f * size.x, -0.5f * size.y, -0.5f * size.z,  0.0f,  0.0f, -1.0f,
		+0.5f * size.x, +0.5f * size.y, -0.5f * size.z,  0.0f,  0.0f, -1.0f,
		+0.5f * size.x, +0.5f * size.y, -0.5f * size.z,  0.0f,  0.0f, -1.0f,
		-0.5f * size.x, +0.5f * size.y, -0.5f * size.z,  0.0f,  0.0f, -1.0f,
		-0.5f * size.x, -0.5f * size.y, -0.5f * size.z,  0.0f,  0.0f, -1.0f,
														 
		-0.5f * size.x, -0.5f * size.y, +0.5f * size.z,  0.0f,  0.0f,  1.0f,
		+0.5f * size.x, -0.5f * size.y, +0.5f * size.z,  0.0f,  0.0f,  1.0f,
		+0.5f * size.x, +0.5f * size.y, +0.5f * size.z,  0.0f,  0.0f,  1.0f,
		+0.5f * size.x, +0.5f * size.y, +0.5f * size.z,  0.0f,  0.0f,  1.0f,
		-0.5f * size.x, +0.5f * size.y, +0.5f * size.z,  0.0f,  0.0f,  1.0f,
		-0.5f * size.x, -0.5f * size.y, +0.5f * size.z,  0.0f,  0.0f,  1.0f,
			  			
		-0.5f * size.x, +0.5f * size.y, +0.5f * size.z, -1.0f,  0.0f,  0.0f,
		-0.5f * size.x, +0.5f * size.y, -0.5f * size.z, -1.0f,  0.0f,  0.0f,
		-0.5f * size.x, -0.5f * size.y, -0.5f * size.z, -1.0f,  0.0f,  0.0f,
		-0.5f * size.x, -0.5f * size.y, -0.5f * size.z, -1.0f,  0.0f,  0.0f,
		-0.5f * size.x, -0.5f * size.y, +0.5f * size.z, -1.0f,  0.0f,  0.0f,
		-0.5f * size.x, +0.5f * size.y, +0.5f * size.z, -1.0f,  0.0f,  0.0f,
			 				
		+0.5f * size.x, +0.5f * size.y, +0.5f * size.z,  1.0f,  0.0f,  0.0f,
		+0.5f * size.x, +0.5f * size.y, -0.5f * size.z,  1.0f,  0.0f,  0.0f,
		+0.5f * size.x, -0.5f * size.y, -0.5f * size.z,  1.0f,  0.0f,  0.0f,
		+0.5f * size.x, -0.5f * size.y, -0.5f * size.z,  1.0f,  0.0f,  0.0f,
		+0.5f * size.x, -0.5f * size.y, +0.5f * size.z,  1.0f,  0.0f,  0.0f,
		+0.5f * size.x, +0.5f * size.y, +0.5f * size.z,  1.0f,  0.0f,  0.0f,
						
		-0.5f * size.x, -0.5f * size.y, -0.5f * size.z,  0.0f, -1.0f,  0.0f,
		+0.5f * size.x, -0.5f * size.y, -0.5f * size.z,  0.0f, -1.0f,  0.0f,
		+0.5f * size.x, -0.5f * size.y, +0.5f * size.z,  0.0f, -1.0f,  0.0f,
		+0.5f * size.x, -0.5f * size.y, +0.5f * size.z,  0.0f, -1.0f,  0.0f,
		-0.5f * size.x, -0.5f * size.y, +0.5f * size.z,  0.0f, -1.0f,  0.0f,
		-0.5f * size.x, -0.5f * size.y, -0.5f * size.z,  0.0f, -1.0f,  0.0f,
			 											 
		-0.5f * size.x, +0.5f * size.y, -0.5f * size.z,  0.0f,  1.0f,  0.0f,
		+0.5f * size.x, +0.5f * size.y, -0.5f * size.z,  0.0f,  1.0f,  0.0f,
		+0.5f * size.x, +0.5f * size.y, +0.5f * size.z,  0.0f,  1.0f,  0.0f,
		+0.5f * size.x, +0.5f * size.y, +0.5f * size.z,  0.0f,  1.0f,  0.0f,
		-0.5f * size.x, +0.5f * size.y, +0.5f * size.z,  0.0f,  1.0f,  0.0f,
		-0.5f * size.x, +0.5f * size.y, -0.5f * size.z,  0.0f,  1.0f,  0.0f
	};
	return vertices;
}
