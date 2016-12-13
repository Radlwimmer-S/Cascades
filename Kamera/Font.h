#pragma once
#include <glm/vec2.hpp>
#include "Character.h"
#include <map>

class Font
{
public:
	Font(char* font, glm::ivec2 size);
	~Font();
	const Character& GetChar(const GLchar c)const;
	glm::ivec2 GetSize() const;

protected:
	std::map<GLchar, Character> Characters;
};

