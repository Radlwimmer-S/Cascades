#pragma once
#include "Light.h"
class DirectionalLight : public Light
{
public:
	DirectionalLight(glm::vec3 position, glm::vec3 color, Shader& shadowShader, int farPlane, int nearPlane);
	virtual ~DirectionalLight();

	void UpdateUniforms(Shader& shader, LightIndexer& indizes) override;

	void PreRender() const override;
	void RenderDebug(Shader& shader) const override;

protected:
	glm::mat4 GetProjection() const override;
	glm::mat4 GetView() const;
	glm::mat4 GetShadowMatrix() const;

	GLfloat m_nearPlane;
};

