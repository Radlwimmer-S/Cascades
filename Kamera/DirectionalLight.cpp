#include "DirectionalLight.h"
#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include "Enums.h"
#include "Box.h"


DirectionalLight::DirectionalLight(glm::vec3 position, glm::vec3 color, Shader& shadowShader, GLfloat farPlane, GLfloat nearPlane) : Light(position, glm::quat(), color, shadowShader, nearPlane, farPlane)
{
	glGenFramebuffers(1, &depthMapFBO);

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	m_debugCube = new Model(m_position, glm::quat(), Box::GetTrisP(glm::vec3(0.1f, 0.1f, 1)), 12, m_color);
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::UpdateUniforms(Shader& shader, int lightIndex, int textureIndex)
{
	Light::UpdateUniforms(shader, lightIndex, textureIndex);

	GLint lightSpaceMatrixLoc = glGetUniformLocation(shader.Program, ("Lights[" + std::to_string(lightIndex) + "].lightSpaceMatrix").c_str());
	glUniformMatrix4fv(lightSpaceMatrixLoc, 1, GL_FALSE, glm::value_ptr(GetShadowMatrix()));
	glCheckError();

	glActiveTexture(GL_TEXTURE0 + textureIndex);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	GLuint depthMapPos = glGetUniformLocation(shader.Program, ("Lights[" + std::to_string(lightIndex) + "].depthMap").c_str());
	glUniform1i(depthMapPos, textureIndex);
	glCheckError();
}

void DirectionalLight::PreRender() const
{
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	m_shadowShader.Use();

	GLint lightSpaceMatrixLoc = glGetUniformLocation(m_shadowShader.Program, "lightSpaceMatrix");
	glUniformMatrix4fv(lightSpaceMatrixLoc, 1, GL_FALSE, glm::value_ptr(GetShadowMatrix()));
	glCheckError();
}

void DirectionalLight::RenderDebug(Shader& shader) const
{
	m_debugCube->SetPosition(m_position);
	m_debugCube->SetOrientation(glm::conjugate(glm::toQuat(GetView())));
	m_debugCube->Render(shader);
}

int DirectionalLight::GetType()
{
	return DIR_LIGHT;
}

glm::mat4 DirectionalLight::GetProjection() const
{
	return glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, m_nearPlane, m_farPlane);
}

glm::mat4 DirectionalLight::GetView() const
{
	return glm::lookAt(m_position, glm::vec3(0), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 DirectionalLight::GetShadowMatrix() const
{
	return GetProjection() * GetView();
}
