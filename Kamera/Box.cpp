#include "Box.h"
#include "Texture.h"

Triangle* Box::GetTris(glm::vec3 size, glm::vec2 textureRepeat)
{
	Triangle* tris = new Triangle[12]
	{
		//Front
		Triangle(Vertex(glm::vec3(+0.5f * size.x, -0.5f * size.y, -0.5f * size.z), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec2( 0.0f * textureRepeat.s, 0.0f * textureRepeat.t)), 
				 Vertex(glm::vec3(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec2( 1.0f * textureRepeat.s, 0.0f * textureRepeat.t)), 
				 Vertex(glm::vec3(-0.5f * size.x, +0.5f * size.y, -0.5f * size.z), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec2( 1.0f * textureRepeat.s, 1.0f * textureRepeat.t))),
		Triangle(Vertex(glm::vec3(-0.5f * size.x, +0.5f * size.y, -0.5f * size.z), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec2( 1.0f * textureRepeat.s, 1.0f * textureRepeat.t)), 
				 Vertex(glm::vec3(+0.5f * size.x, +0.5f * size.y, -0.5f * size.z), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec2( 0.0f * textureRepeat.s, 1.0f * textureRepeat.t)), 
				 Vertex(glm::vec3(+0.5f * size.x, -0.5f * size.y, -0.5f * size.z), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec2( 0.0f * textureRepeat.s, 0.0f * textureRepeat.t))),
		//Back												   								   
		Triangle(Vertex(glm::vec3(-0.5f * size.x, -0.5f * size.y, +0.5f * size.z), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2( 0.0f * textureRepeat.s, 0.0f * textureRepeat.t)),
				 Vertex(glm::vec3(+0.5f * size.x, -0.5f * size.y, +0.5f * size.z), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2( 1.0f * textureRepeat.s, 0.0f * textureRepeat.t)),
				 Vertex(glm::vec3(+0.5f * size.x, +0.5f * size.y, +0.5f * size.z), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2( 1.0f * textureRepeat.s, 1.0f * textureRepeat.t))),
		Triangle(Vertex(glm::vec3(+0.5f * size.x, +0.5f * size.y, +0.5f * size.z), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2( 1.0f * textureRepeat.s, 1.0f * textureRepeat.t)),
				 Vertex(glm::vec3(-0.5f * size.x, +0.5f * size.y, +0.5f * size.z), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2( 0.0f * textureRepeat.s, 1.0f * textureRepeat.t)),
				 Vertex(glm::vec3(-0.5f * size.x, -0.5f * size.y, +0.5f * size.z), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2( 0.0f * textureRepeat.s, 0.0f * textureRepeat.t))),
		//Left												   								   
		Triangle(Vertex(glm::vec3(-0.5f * size.x, +0.5f * size.y, +0.5f * size.z), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2( 0.0f * textureRepeat.s, 0.0f * textureRepeat.t)),
				 Vertex(glm::vec3(-0.5f * size.x, +0.5f * size.y, -0.5f * size.z), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2( 1.0f * textureRepeat.s, 0.0f * textureRepeat.t)),
				 Vertex(glm::vec3(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2( 1.0f * textureRepeat.s, 1.0f * textureRepeat.t))),
		Triangle(Vertex(glm::vec3(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2( 1.0f * textureRepeat.s, 1.0f * textureRepeat.t)),
				 Vertex(glm::vec3(-0.5f * size.x, -0.5f * size.y, +0.5f * size.z), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2( 0.0f * textureRepeat.s, 1.0f * textureRepeat.t)),
				 Vertex(glm::vec3(-0.5f * size.x, +0.5f * size.y, +0.5f * size.z), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2( 0.0f * textureRepeat.s, 0.0f * textureRepeat.t))),
		//Right																		   		 						  
		Triangle(Vertex(glm::vec3(+0.5f * size.x, +0.5f * size.y, -0.5f * size.z), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2( 0.0f * textureRepeat.s, 0.0f * textureRepeat.t)),
				 Vertex(glm::vec3(+0.5f * size.x, +0.5f * size.y, +0.5f * size.z), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2( 1.0f * textureRepeat.s, 0.0f * textureRepeat.t)),
				 Vertex(glm::vec3(+0.5f * size.x, -0.5f * size.y, +0.5f * size.z), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2( 1.0f * textureRepeat.s, 1.0f * textureRepeat.t))),
		Triangle(Vertex(glm::vec3(+0.5f * size.x, -0.5f * size.y, +0.5f * size.z), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2( 1.0f * textureRepeat.s, 1.0f * textureRepeat.t)),
				 Vertex(glm::vec3(+0.5f * size.x, -0.5f * size.y, -0.5f * size.z), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2( 0.0f * textureRepeat.s, 1.0f * textureRepeat.t)),
				 Vertex(glm::vec3(+0.5f * size.x, +0.5f * size.y, -0.5f * size.z), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2( 0.0f * textureRepeat.s, 0.0f * textureRepeat.t))),
		//Bottom																		   	 							  
		Triangle(Vertex(glm::vec3(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec2( 0.0f * textureRepeat.s, 0.0f * textureRepeat.t)),
				 Vertex(glm::vec3(+0.5f * size.x, -0.5f * size.y, -0.5f * size.z), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec2( 1.0f * textureRepeat.s, 0.0f * textureRepeat.t)),
				 Vertex(glm::vec3(+0.5f * size.x, -0.5f * size.y, +0.5f * size.z), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec2( 1.0f * textureRepeat.s, 1.0f * textureRepeat.t))),
		Triangle(Vertex(glm::vec3(+0.5f * size.x, -0.5f * size.y, +0.5f * size.z), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec2( 1.0f * textureRepeat.s, 1.0f * textureRepeat.t)),
				 Vertex(glm::vec3(-0.5f * size.x, -0.5f * size.y, +0.5f * size.z), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec2( 0.0f * textureRepeat.s, 1.0f * textureRepeat.t)),
				 Vertex(glm::vec3(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec2( 0.0f * textureRepeat.s, 0.0f * textureRepeat.t))),
		//Top																		   		 						  
		Triangle(Vertex(glm::vec3(+0.5f * size.x, +0.5f * size.y, -0.5f * size.z), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2( 0.0f * textureRepeat.s, 0.0f * textureRepeat.t)),
				 Vertex(glm::vec3(-0.5f * size.x, +0.5f * size.y, -0.5f * size.z), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2( 1.0f * textureRepeat.s, 0.0f * textureRepeat.t)),
				 Vertex(glm::vec3(-0.5f * size.x, +0.5f * size.y, +0.5f * size.z), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2( 1.0f * textureRepeat.s, 1.0f * textureRepeat.t))),
		Triangle(Vertex(glm::vec3(-0.5f * size.x, +0.5f * size.y, +0.5f * size.z), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2( 1.0f * textureRepeat.s, 1.0f * textureRepeat.t)),
				 Vertex(glm::vec3(+0.5f * size.x, +0.5f * size.y, +0.5f * size.z), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2( 0.0f * textureRepeat.s, 1.0f * textureRepeat.t)),
				 Vertex(glm::vec3(+0.5f * size.x, +0.5f * size.y, -0.5f * size.z), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2( 0.0f * textureRepeat.s, 0.0f * textureRepeat.t)))
	};
	return tris;
}

Triangle* Box::GetTrisP(glm::vec3 size)
{
	Triangle* tris = new Triangle[12]
	{
		Triangle(Vertex(glm::vec3(+0.5f * size.x, -0.5f * size.y, -0.5f * size.z)),
				 Vertex(glm::vec3(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z)),
				 Vertex(glm::vec3(-0.5f * size.x, +0.5f * size.y, -0.5f * size.z))),
		Triangle(Vertex(glm::vec3(-0.5f * size.x, +0.5f * size.y, -0.5f * size.z)),
				 Vertex(glm::vec3(+0.5f * size.x, +0.5f * size.y, -0.5f * size.z)),
				 Vertex(glm::vec3(+0.5f * size.x, -0.5f * size.y, -0.5f * size.z))),
															   					
		Triangle(Vertex(glm::vec3(-0.5f * size.x, -0.5f * size.y, +0.5f * size.z)),
				 Vertex(glm::vec3(+0.5f * size.x, -0.5f * size.y, +0.5f * size.z)),
				 Vertex(glm::vec3(+0.5f * size.x, +0.5f * size.y, +0.5f * size.z))),
		Triangle(Vertex(glm::vec3(+0.5f * size.x, +0.5f * size.y, +0.5f * size.z)),
				 Vertex(glm::vec3(-0.5f * size.x, +0.5f * size.y, +0.5f * size.z)),
				 Vertex(glm::vec3(-0.5f * size.x, -0.5f * size.y, +0.5f * size.z))),
															   					
		Triangle(Vertex(glm::vec3(-0.5f * size.x, +0.5f * size.y, +0.5f * size.z)),
				 Vertex(glm::vec3(-0.5f * size.x, +0.5f * size.y, -0.5f * size.z)),
				 Vertex(glm::vec3(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z))),
		Triangle(Vertex(glm::vec3(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z)),
				 Vertex(glm::vec3(-0.5f * size.x, -0.5f * size.y, +0.5f * size.z)),
				 Vertex(glm::vec3(-0.5f * size.x, +0.5f * size.y, +0.5f * size.z))),
																				
		Triangle(Vertex(glm::vec3(+0.5f * size.x, +0.5f * size.y, -0.5f * size.z)),
				 Vertex(glm::vec3(+0.5f * size.x, +0.5f * size.y, +0.5f * size.z)),
				 Vertex(glm::vec3(+0.5f * size.x, -0.5f * size.y, +0.5f * size.z))),
		Triangle(Vertex(glm::vec3(+0.5f * size.x, -0.5f * size.y, +0.5f * size.z)),
				 Vertex(glm::vec3(+0.5f * size.x, -0.5f * size.y, -0.5f * size.z)),
				 Vertex(glm::vec3(+0.5f * size.x, +0.5f * size.y, -0.5f * size.z))),
																				
		Triangle(Vertex(glm::vec3(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z)),
				 Vertex(glm::vec3(+0.5f * size.x, -0.5f * size.y, -0.5f * size.z)),
				 Vertex(glm::vec3(+0.5f * size.x, -0.5f * size.y, +0.5f * size.z))),
		Triangle(Vertex(glm::vec3(+0.5f * size.x, -0.5f * size.y, +0.5f * size.z)),
				 Vertex(glm::vec3(-0.5f * size.x, -0.5f * size.y, +0.5f * size.z)),
				 Vertex(glm::vec3(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z))),
																				
		Triangle(Vertex(glm::vec3(+0.5f * size.x, +0.5f * size.y, -0.5f * size.z)),
				 Vertex(glm::vec3(-0.5f * size.x, +0.5f * size.y, -0.5f * size.z)),
				 Vertex(glm::vec3(-0.5f * size.x, +0.5f * size.y, +0.5f * size.z))),
		Triangle(Vertex(glm::vec3(-0.5f * size.x, +0.5f * size.y, +0.5f * size.z)),
				 Vertex(glm::vec3(+0.5f * size.x, +0.5f * size.y, +0.5f * size.z)),
				 Vertex(glm::vec3(+0.5f * size.x, +0.5f * size.y, -0.5f * size.z)))
	};
	return tris;
}
