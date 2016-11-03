#include "Light.h"

Light::Light(glm::vec3 position, glm::quat orientation, glm::vec3 color, Shader& shadowShader, int farPlane) : BaseObject(position, orientation), m_color(color), m_shadowShader(shadowShader), m_farPlane(farPlane)
{
}

Light::~Light()
{
}

void Light::PostRender() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
