#pragma once
#include "DirectionalLight.h"
class SpotLight :
	public DirectionalLight
{
public:
	SpotLight(glm::vec3 position, glm::vec3 color, Shader& shadowShader, GLfloat fovy, GLfloat farPlane, GLfloat nearPlane = 1);
	virtual ~SpotLight();

	void UpdateUniforms(Shader& shader, LightIndexer& indizes) override;

protected:
	glm::mat4 GetProjection() const override;
	float m_fovy;
};

