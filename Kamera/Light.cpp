#include "Light.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include <string>
#include "Global.h"
#include "Box.h"

Light::Light(glm::vec3 position, glm::vec3 color, GLfloat farPlane) : BaseObject(position), m_color(color), m_debugCube(new Model(m_position, glm::quat(), Box::GetV(glm::vec3(0.1f, 0.1f, 0.1f)), 36, V, m_color)), m_farPlane(farPlane)
{
	glGenFramebuffers(1, &depthMapFBO);

	glGenTextures(1, &depthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	for (GLuint i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Light::~Light()
{
}

void Light::PreRender(Shader& shader) const
{
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	std::vector<glm::mat4> shadowMatrices = GetShadowMatrices();
	for (GLuint i = 0; i < 6; ++i)
	{
		GLint shadowMatrixLoc = glGetUniformLocation(shader.Program, ("shadowMatrices[" + std::to_string(i) + "]").c_str());
		glUniformMatrix4fv(shadowMatrixLoc, 1, GL_FALSE, glm::value_ptr(shadowMatrices[i]));
		glCheckError();
	}

	GLint lightPosLoc = glGetUniformLocation(shader.Program, "lightPos");
	glUniform3f(lightPosLoc, m_position.x, m_position.y, m_position.z);
	glCheckError();
	
	GLint farPlaneLoc = glGetUniformLocation(shader.Program, "far_plane");
	glUniform1f(farPlaneLoc, GetFarPlane());
	glCheckError();
}

void Light::PostRender() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Light::RenderCube(Shader& shader) const
{
	m_debugCube->SetPosition(m_position);
	m_debugCube->Render(shader);
}

glm::vec3 Light::GetColor() const
{
	return m_color;
}
std::vector<glm::mat4> Light::GetShadowMatrices() const
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

glm::mat4 Light::GetProjection() const
{
	GLfloat aspect = (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT;
	GLfloat near = 1.0f;
	return  glm::perspective(glm::radians(90.0f), aspect, near, GetFarPlane());
}
