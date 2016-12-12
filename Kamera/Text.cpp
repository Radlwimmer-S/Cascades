#include "Text.h"
#include <GL/glew.h>
#include "Character.h"
#include "Font.h"
#include "Shader.h"

Text::Text(std::string string, Font& font, glm::vec2 pos, float scale, glm::vec3 color) : string_(string), font_(font), pos_(pos), scale_(scale), color_(color)
{
	if (vao_ == 0)
	{
		// Configure VAO/VBO for texture quads
		glGenVertexArrays(1, &vao_);
		glGenBuffers(1, &vbo_);
		glBindVertexArray(vao_);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}

GLuint Text::vao_ = 0;
GLuint Text::vbo_ = 0;

Text::~Text()
{
}

void Text::Render(Shader& shader) const
{
	glUniform3f(glGetUniformLocation(shader.Program, "textColor"), color_.x, color_.y, color_.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(vao_);

	glm::vec2 charPos(pos_);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = string_.begin(); c != string_.end(); ++c)
	{
		Character ch = font_.GetChar(*c);
		auto x = *c;

		GLfloat xpos = pos_.x + ch.Bearing.x * scale_;
		GLfloat ypos = pos_.y - (ch.Size.y - ch.Bearing.y) * scale_;

		GLfloat w = ch.Size.x * scale_;
		GLfloat h = ch.Size.y * scale_;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, vbo_);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		charPos.x += (ch.Advance >> 6) * scale_; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
