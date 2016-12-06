#pragma once
#include "DirectionalLight.h"
class SpotLight :
	public DirectionalLight
{
public:
	SpotLight(glm::vec3 position, glm::quat orientation, glm::vec3 color, Shader& shadowShader, GLfloat fovy, GLfloat farPlane, GLfloat nearPlane = 0.1f);
	virtual ~SpotLight();

protected:
	int GetType() override;
	glm::mat4 GetProjection() const override;
	glm::mat4 GetView() const override;
	float m_fovy;
};

