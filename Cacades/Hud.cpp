#include "Hud.h"
#include "Global.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Font.h"
#include "MeasuringTool.h"

Hud::Hud(Font& font, Shader& shader) : m_font(font), m_shader(shader),
m_infoText(Text("", m_font, glm::vec2(25, SCREEN_HEIGHT - 50), 1, glm::vec3(1, 1, 0))),
m_crosshair(Text("+", m_font, glm::vec2(0, 0), 2, glm::vec3(1, 0, 0)))
{
	glm::vec2 charPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	Character plus = font.GetChar('+');
	GLfloat xpos = charPos.x - (plus.Bearing.x + plus.Size.x / 2.f) * m_crosshair.GetScale();
	GLfloat ypos = charPos.y - (plus.Bearing.y - plus.Size.y / 2.f) * m_crosshair.GetScale();
	m_crosshair.SetPosition(glm::vec2(xpos, ypos));
}

Hud::~Hud()
{
}

void Hud::Update(int fps, const RenderInfo& renderInfo)
{
	std::stringstream ss;
	ss.precision(2);

	ss << "FPS: " << fps << std::endl << std::endl;
	ss << "Marching-Cubes:" << std::endl;
	ss << "  Seed: " << renderInfo.Seed << std::endl;
	ss << "  Noise Scale: " << renderInfo.NoiseScale << std::endl;
	ss << "  Layer: " << renderInfo.StartLayer << std::endl;
	ss << "  Resolution: " << renderInfo.Resolution.x << "/" << renderInfo.Resolution.y << "/" << renderInfo.Resolution.z << std::endl;
	ss << "ShadowMode: " << ((renderInfo.ShadowMode == PcfShadows) ? "PCF" : (renderInfo.ShadowMode == VsmShadows) ? "VSM" : "Hard") << std::endl;
	m_infoText.SetString(ss.str());
}

void Hud::Render() const
{
	m_shader.Use();

	glm::mat4 proj = glm::ortho(0.0f, static_cast<GLfloat>(SCREEN_WIDTH), 0.0f, static_cast<GLfloat>(SCREEN_HEIGHT));
	GLuint projLocation = glGetUniformLocation(m_shader.Program, "projection");
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(proj));
	glCheckError();

	m_infoText.Render(m_shader);
	m_crosshair.Render(m_shader);
}
