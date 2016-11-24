#pragma once
#include <glm/gtx/norm.hpp>
#include <GL/glew.h>
#include "Vertex.h"

struct Triangle
{
	Triangle(Vertex v0, Vertex v1, Vertex v2);
	Triangle(Vertex vertices[3]);

	Vertex Vertices[3];
};
