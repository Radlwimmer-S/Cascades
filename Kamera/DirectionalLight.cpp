#include "DirectionalLight.h"
#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>


DirectionalLight::DirectionalLight(glm::vec3 position, glm::vec3 color, Shader& shadowShader, int farPlane, int nearPlane) : Light(position, glm::quat(), color, shadowShader, farPlane), m_nearPlane(nearPlane)
{
	glGenFramebuffers(1, &depthMapFBO);
	// - Create depth texture
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
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::UpdateUniforms(Shader& shader, int lightId, int textureId)
{
	GLint lightPosLoc = glGetUniformLocation(shader.Program, ("DirLight[" + std::to_string(lightId) + "].Pos").c_str());
	glUniform3f(lightPosLoc, -m_position.x, -m_position.y, -m_position.z);
	glCheckError();

	GLint lightColorLoc = glGetUniformLocation(shader.Program, ("DirLight[" + std::to_string(lightId) + "].Color").c_str());
	glUniform3f(lightColorLoc, m_color.r, m_color.g, m_color.b);
	glCheckError();

	GLint lightSpaceMatrixLoc = glGetUniformLocation(shader.Program, ("DirLight[" + std::to_string(lightId) + "].lightSpaceMatrix").c_str());
	glUniformMatrix4fv(lightSpaceMatrixLoc, 1, GL_FALSE, glm::value_ptr(GetShadowMatrix())); 
	glCheckError();

	glActiveTexture(GL_TEXTURE0 + textureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap);
	GLuint depthMapPos = glGetUniformLocation(shader.Program, ("DirLight[" + std::to_string(lightId) + "].depthMap").c_str());
	glUniform1i(depthMapPos, textureId);
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
{}

glm::mat4 DirectionalLight::GetProjection() const
{
	return glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, m_nearPlane, m_farPlane);
}

glm::mat4 DirectionalLight::GetShadowMatrix() const
{
	return GetProjection() * glm::lookAt(m_position, glm::vec3(0), glm::vec3(0.0f, 1.0f, 0.0f));
}
