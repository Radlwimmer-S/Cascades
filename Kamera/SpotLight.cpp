#include "SpotLight.h"
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

SpotLight::SpotLight(glm::vec3 position, glm::vec3 color, Shader& shadowShader, GLfloat fovy, GLfloat farPlane, GLfloat nearPlane) : DirectionalLight(position, color, shadowShader, farPlane, nearPlane), m_fovy(fovy)
{
	/*glBindTexture(GL_TEXTURE_2D, depthMap);
	GLfloat borderColor[] = { 0.0, 0.0, 0.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindTexture(GL_TEXTURE_2D, 0);*/
}

SpotLight::~SpotLight()
{
}

void SpotLight::UpdateUniforms(Shader& shader, LightIndexer& indizes)
{
	GLint lightPosLoc = glGetUniformLocation(shader.Program, ("SpotLight[" + std::to_string(indizes.SpotIndex) + "].Pos").c_str());
	glUniform3f(lightPosLoc, m_position.x, m_position.y, m_position.z);
	glCheckError();

	GLint lightColorLoc = glGetUniformLocation(shader.Program, ("SpotLight[" + std::to_string(indizes.SpotIndex) + "].Color").c_str());
	glUniform3f(lightColorLoc, m_color.r, m_color.g, m_color.b);
	glCheckError();

	GLint lightSpaceMatrixLoc = glGetUniformLocation(shader.Program, ("SpotLight[" + std::to_string(indizes.SpotIndex) + "].lightSpaceMatrix").c_str());
	glUniformMatrix4fv(lightSpaceMatrixLoc, 1, GL_FALSE, glm::value_ptr(GetShadowMatrix()));
	glCheckError();

	glActiveTexture(GL_TEXTURE0 + indizes.TextureIndex);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	GLuint depthMapPos = glGetUniformLocation(shader.Program, ("SpotLight[" + std::to_string(indizes.SpotIndex) + "].depthMap").c_str());
	glUniform1i(depthMapPos, indizes.TextureIndex);
	glCheckError();

	++indizes.SpotIndex;
	++indizes.TextureIndex;
}

glm::mat4 SpotLight::GetProjection() const
{
	GLfloat aspect = (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT;
	return  glm::perspective(glm::radians(m_fovy), aspect, m_nearPlane, m_farPlane);
}
