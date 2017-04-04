#pragma once
#include <string>
#include <glm/detail/type_vec2.hpp>
#include <glm/detail/type_vec3.hpp>
#include <GL/glew.h>

class Font;
class Shader;

class Text
{
public:
	Text(std::string string, Font& font, glm::vec2 pos, float scale, glm::vec3 color);
	~Text();

	void Render(Shader& shader) const;

	void SetString(const std::string& string);

	void SetPosition(glm::vec2 pos);
	float GetScale() const;
protected:
	std::string string_;
	Font& font_;
	glm::vec2 pos_;
	float scale_;
	glm::vec3 color_;

	static GLuint vao_, vbo_;
};

