#include "Light.h"
#include "Shader.h"

Light::Light(glm::vec3 position, glm::quat orientation, glm::vec3 color, Shader& shadowShader, int nearPlane, int farPlane) : BaseObject(position, orientation), m_color(color), m_shadowShader(shadowShader), m_farPlane(farPlane), m_nearPlane(nearPlane)
{
}

Light::~Light()
{
}

void Light::UpdateUniforms(Shader& shader, int lightIndex, int textureIndex)
{
	GLint posLoc = glGetUniformLocation(shader.Program, ("Lights[" + std::to_string(lightIndex) + "].Pos").c_str());
	glUniform3f(posLoc, m_position.x, m_position.y, m_position.z);
	glCheckError();

	GLint colorLoc = glGetUniformLocation(shader.Program, ("Lights[" + std::to_string(lightIndex) + "].Color").c_str());
	glUniform3f(colorLoc, m_color.r, m_color.g, m_color.b);
	glCheckError();

	GLint typePos = glGetUniformLocation(shader.Program, ("Lights[" + std::to_string(lightIndex) + "].Type").c_str());
	glUniform1i(typePos, GetType());
	glCheckError();

	GLint enabledPos = glGetUniformLocation(shader.Program, ("Lights[" + std::to_string(lightIndex) + "].IsEnabled").c_str());
	glUniform1i(enabledPos, m_isEnabled);
	glCheckError();

	GLint nearPlaneLoc = glGetUniformLocation(shader.Program, ("Lights[" + std::to_string(lightIndex) + "].near_plane").c_str());
	glUniform1f(nearPlaneLoc, m_nearPlane);
	glCheckError();

	GLint farPlaneLoc = glGetUniformLocation(shader.Program, ("Lights[" + std::to_string(lightIndex) + "].far_plane").c_str());
	glUniform1f(farPlaneLoc, m_farPlane);
	glCheckError();
}

void Light::PostRender() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
