#include "PointLight.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include <string>
#include "Global.h"
#include "Box.h"

PointLight::PointLight(glm::vec3 position, glm::vec3 color, Shader& shadowShader, GLfloat farPlane, GLfloat nearPlane) : Light(position, glm::quat(), color, shadowShader, nearPlane, farPlane)
{
	glGenFramebuffers(1, &shadowMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);

	textureType = GL_TEXTURE_CUBE_MAP;

	glGenTextures(1, &shadowMap);
	glBindTexture(textureType, shadowMap);
	for (GLuint i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RG16F, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_RG, GL_FLOAT, nullptr);

	glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glCheckError();

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, shadowMap, 0);
	glBindTexture(textureType, 0);
	glCheckError();

	glGenTextures(1, &depthAttachment);
	glBindTexture(textureType, depthAttachment);
	for (GLuint i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glCheckError();

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthAttachment, 0);
	glBindTexture(textureType, 0);
	glCheckError();

	glCheckFrameBuffer();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCheckError();

	m_debugCube = new Model(m_position, glm::quat(), Box::GetTris(glm::vec3(.1f)), 12, m_color, NoNormals);
}

PointLight::~PointLight()
{
}

void PointLight::UpdateUniforms(const Shader& shader, int lightIndex, int textureIndex)
{
	Light::UpdateUniforms(shader, lightIndex, textureIndex);

	glActiveTexture(GL_TEXTURE0 + textureIndex);
	glBindTexture(textureType, shadowMap);
	GLuint depthCubePos = glGetUniformLocation(shader.Program, ("Lights[" + std::to_string(lightIndex) + "].depthCube").c_str());
	glUniform1i(depthCubePos, textureIndex);
	glCheckError();
}

void PointLight::PreRender() const
{
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_shadowShader.Use();

	std::vector<glm::mat4> shadowMatrices = GetShadowMatrices();
	for (GLuint i = 0; i < 6; ++i)
	{
		GLint shadowMatrixLoc = glGetUniformLocation(m_shadowShader.Program, ("shadowMatrices[" + std::to_string(i) + "]").c_str());
		glUniformMatrix4fv(shadowMatrixLoc, 1, GL_FALSE, glm::value_ptr(shadowMatrices[i]));
		glCheckError();
	}

	GLint lightPosLoc = glGetUniformLocation(m_shadowShader.Program, "lightPos");
	glUniform3f(lightPosLoc, m_position.x, m_position.y, m_position.z);
	glCheckError();

	GLint farPlaneLoc = glGetUniformLocation(m_shadowShader.Program, "far_plane");
	glUniform1f(farPlaneLoc, m_farPlane);
	glCheckError();
}

void PointLight::RenderDebug(Shader& shader) const
{
	m_debugCube->SetPosition(m_position);
	m_debugCube->Render(shader);
}

LightType PointLight::GetType()
{
	return LightType::Point;
}

std::vector<glm::mat4> PointLight::GetShadowMatrices() const
{
	glm::mat4 shadowProj = GetProjection();
	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_position, m_position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0))); //right
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_position, m_position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0))); //left
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_position, m_position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0))); //top
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_position, m_position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0))); //bottom
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_position, m_position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0))); //near
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_position, m_position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0))); //far
	return shadowTransforms;
}

glm::mat4 PointLight::GetProjection() const
{
	GLfloat aspect = (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT;
	return  glm::perspective(glm::radians(90.0f), aspect, m_nearPlane, m_farPlane);
}
