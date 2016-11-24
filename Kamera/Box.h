#pragma once
#include "Model.h"

class Box 
{
public:
	static Triangle* GetTris(glm::vec3 size = glm::vec3(1), glm::vec2 textureRepeat = glm::vec2(1));
	static Triangle* GetTrisP(glm::vec3 size = glm::vec3(1));
};

