#include "Box.h"
#include "Texture.h"

GLfloat* Box::GetVNT(glm::vec3 size)
{
	GLfloat* vertices = new GLfloat[36 * 8]{
		-0.5f * size.x, -0.5f * size.y, -0.5f * size.z,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
		+0.5f * size.x, -0.5f * size.y, -0.5f * size.z,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
		+0.5f * size.x, +0.5f * size.y, -0.5f * size.z,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		+0.5f * size.x, +0.5f * size.y, -0.5f * size.z,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f * size.x, +0.5f * size.y, -0.5f * size.z,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f * size.x, -0.5f * size.y, -0.5f * size.z,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
														 
		-0.5f * size.x, -0.5f * size.y, +0.5f * size.z,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
		+0.5f * size.x, -0.5f * size.y, +0.5f * size.z,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
		+0.5f * size.x, +0.5f * size.y, +0.5f * size.z,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
		+0.5f * size.x, +0.5f * size.y, +0.5f * size.z,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
		-0.5f * size.x, +0.5f * size.y, +0.5f * size.z,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
		-0.5f * size.x, -0.5f * size.y, +0.5f * size.z,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
			  			
		-0.5f * size.x, +0.5f * size.y, +0.5f * size.z, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		-0.5f * size.x, +0.5f * size.y, -0.5f * size.z, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-0.5f * size.x, -0.5f * size.y, -0.5f * size.z, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		-0.5f * size.x, -0.5f * size.y, -0.5f * size.z, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		-0.5f * size.x, -0.5f * size.y, +0.5f * size.z, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		-0.5f * size.x, +0.5f * size.y, +0.5f * size.z, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
			 				
		+0.5f * size.x, +0.5f * size.y, +0.5f * size.z,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		+0.5f * size.x, +0.5f * size.y, -0.5f * size.z,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		+0.5f * size.x, -0.5f * size.y, -0.5f * size.z,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		+0.5f * size.x, -0.5f * size.y, -0.5f * size.z,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		+0.5f * size.x, -0.5f * size.y, +0.5f * size.z,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		+0.5f * size.x, +0.5f * size.y, +0.5f * size.z,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
						
		-0.5f * size.x, -0.5f * size.y, -0.5f * size.z,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
		+0.5f * size.x, -0.5f * size.y, -0.5f * size.z,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		+0.5f * size.x, -0.5f * size.y, +0.5f * size.z,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
		+0.5f * size.x, -0.5f * size.y, +0.5f * size.z,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
		-0.5f * size.x, -0.5f * size.y, +0.5f * size.z,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
		-0.5f * size.x, -0.5f * size.y, -0.5f * size.z,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
			 											 
		-0.5f * size.x, +0.5f * size.y, -0.5f * size.z,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
		+0.5f * size.x, +0.5f * size.y, -0.5f * size.z,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		+0.5f * size.x, +0.5f * size.y, +0.5f * size.z,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
		+0.5f * size.x, +0.5f * size.y, +0.5f * size.z,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
		-0.5f * size.x, +0.5f * size.y, +0.5f * size.z,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
		-0.5f * size.x, +0.5f * size.y, -0.5f * size.z,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f
	};
	return vertices;
}

GLfloat* Box::GetVN(glm::vec3 size)
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

GLfloat* Box::GetV(glm::vec3 size)
{
	GLfloat* vertices = new GLfloat[36 * 3]{
		-0.5f * size.x, -0.5f * size.y, -0.5f * size.z, 
		+0.5f * size.x, -0.5f * size.y, -0.5f * size.z, 
		+0.5f * size.x, +0.5f * size.y, -0.5f * size.z, 
		+0.5f * size.x, +0.5f * size.y, -0.5f * size.z, 
		-0.5f * size.x, +0.5f * size.y, -0.5f * size.z, 
		-0.5f * size.x, -0.5f * size.y, -0.5f * size.z, 

		-0.5f * size.x, -0.5f * size.y, +0.5f * size.z, 
		+0.5f * size.x, -0.5f * size.y, +0.5f * size.z, 
		+0.5f * size.x, +0.5f * size.y, +0.5f * size.z, 
		+0.5f * size.x, +0.5f * size.y, +0.5f * size.z, 
		-0.5f * size.x, +0.5f * size.y, +0.5f * size.z, 
		-0.5f * size.x, -0.5f * size.y, +0.5f * size.z, 

		-0.5f * size.x, +0.5f * size.y, +0.5f * size.z, 
		-0.5f * size.x, +0.5f * size.y, -0.5f * size.z, 
		-0.5f * size.x, -0.5f * size.y, -0.5f * size.z, 
		-0.5f * size.x, -0.5f * size.y, -0.5f * size.z, 
		-0.5f * size.x, -0.5f * size.y, +0.5f * size.z, 
		-0.5f * size.x, +0.5f * size.y, +0.5f * size.z, 

		+0.5f * size.x, +0.5f * size.y, +0.5f * size.z, 
		+0.5f * size.x, +0.5f * size.y, -0.5f * size.z, 
		+0.5f * size.x, -0.5f * size.y, -0.5f * size.z, 
		+0.5f * size.x, -0.5f * size.y, -0.5f * size.z, 
		+0.5f * size.x, -0.5f * size.y, +0.5f * size.z, 
		+0.5f * size.x, +0.5f * size.y, +0.5f * size.z, 

		-0.5f * size.x, -0.5f * size.y, -0.5f * size.z, 
		+0.5f * size.x, -0.5f * size.y, -0.5f * size.z, 
		+0.5f * size.x, -0.5f * size.y, +0.5f * size.z, 
		+0.5f * size.x, -0.5f * size.y, +0.5f * size.z, 
		-0.5f * size.x, -0.5f * size.y, +0.5f * size.z, 
		-0.5f * size.x, -0.5f * size.y, -0.5f * size.z, 

		-0.5f * size.x, +0.5f * size.y, -0.5f * size.z, 
		+0.5f * size.x, +0.5f * size.y, -0.5f * size.z, 
		+0.5f * size.x, +0.5f * size.y, +0.5f * size.z, 
		+0.5f * size.x, +0.5f * size.y, +0.5f * size.z, 
		-0.5f * size.x, +0.5f * size.y, +0.5f * size.z, 
		-0.5f * size.x, +0.5f * size.y, -0.5f * size.z
	};
	return vertices;
}
