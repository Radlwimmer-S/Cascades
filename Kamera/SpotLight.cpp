#include "SpotLight.h"
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

SpotLight::SpotLight(glm::vec3 position, glm::quat orientation, glm::vec3 color, Shader& shadowShader, GLfloat fovy, GLfloat farPlane, GLfloat nearPlane) : DirectionalLight(position, orientation, color, shadowShader, farPlane, nearPlane), m_fovy(fovy)
{
}

SpotLight::~SpotLight()
{
}

int SpotLight::GetType()
{
	return SPOT_LIGHT;
}

glm::mat4 SpotLight::GetProjection() const
{
	GLfloat aspect = (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT;
	return  glm::perspective(glm::radians(m_fovy), aspect, m_nearPlane, m_farPlane);
}

glm::mat4 SpotLight::GetView() const
{
	glm::vec3 center = glm::normalize(glm::vec3(0, 0, -1) * m_orientation);
	glm::vec3 up = glm::normalize(glm::vec3(0, 1, 0) * m_orientation);
	return glm::lookAt(m_position, m_position + center, up);
}
