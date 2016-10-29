#include "Light.h"
#include <glm/gtc/matrix_transform.inl>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

Light::Light(glm::vec3 position, glm::quat rotation, glm::vec3 color, LightType type) : m_position(position), m_rotation(rotation), m_color(color), m_type(type)
{
	glGenFramebuffers(1, &depthMapFBO);

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Light::~Light()
{
}

void Light::Activate(Shader& shader) const
{
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::mat4 lightSpaceMatrix = GetLightSpace();
	GLint lightSpaceMatrixLoc = glGetUniformLocation(shader.Program, "lightSpaceMatrix");
	glUniformMatrix4fv(lightSpaceMatrixLoc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
}

void Light::Deactivate() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::vec3 Light::GetColor() const
{
	return m_color;
}

glm::vec3 Light::GetPosition() const
{
	return m_position;
}

void Light::SetPosition(glm::vec3 position)
{
	m_position = position;
}

void Light::SetRotation(glm::quat rotation)
{
	m_rotation = rotation;
}

glm::mat4 Light::GetLightSpace() const
{
	return   GetProjection() *  GetView();
}

glm::mat4 Light::GetProjection() const
{
	GLfloat near_plane = 1.0f, far_plane = 7.5f;
	return glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
}

glm::mat4 Light::GetView() const
{
	glm::mat4 translation = glm::mat4(1.0f);
	translation = glm::translate(translation, -m_position);
	glm::mat4 rotation = glm::toMat4(glm::normalize(m_rotation));

	return rotation * translation;
}
