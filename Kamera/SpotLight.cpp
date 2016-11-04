#include "SpotLight.h"
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

SpotLight::SpotLight(glm::vec3 position, glm::vec3 color, Shader& shadowShader, GLfloat fovy, GLfloat farPlane, GLfloat nearPlane) : DirectionalLight(position, color, shadowShader, farPlane, nearPlane), m_fovy(fovy)
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
