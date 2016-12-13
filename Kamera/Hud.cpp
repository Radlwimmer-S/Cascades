#include "Hud.h"
#include "Global.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Hud::Hud(Font& font, Shader& shader) : m_font(font), m_shader(shader),
m_fpsText(Text("", m_font, glm::vec2(25, HEIGHT - 50), 1, glm::vec3(1, 1, 0))),
m_aaText(Text("", m_font, glm::vec2(25, HEIGHT - 100), 1, glm::vec3(1, 1, 0)))
{
}

Hud::~Hud()
{
}

void Hud::Update(int fps, const RenderInfo& renderInfo, const AntiAliasingInfo& aaInfo)
{
	std::stringstream ss;

	ss << "FPS: " << fps;
	m_fpsText.SetString(ss.str());

	ss.str("");

	ss << "Samples: " << aaInfo.ColorSamples << std::endl;
	ss << "Coverage: " << aaInfo.CoverageSamples << std::endl;
	ss << "AAMode: " << aaInfo.ParseAAMode();
	m_aaText.SetString(ss.str());
}

void Hud::Render() const
{
	m_shader.Use();
	
	glm::mat4 proj = glm::ortho(0.0f, static_cast<GLfloat>(WIDTH), 0.0f, static_cast<GLfloat>(HEIGHT));
	GLuint projLocation = glGetUniformLocation(m_shader.Program, "projection");
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(proj));
	glCheckError();

	m_fpsText.Render(m_shader);
	m_aaText.Render(m_shader);
}
