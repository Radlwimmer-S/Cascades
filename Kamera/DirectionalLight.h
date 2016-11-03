#pragma once
#include "Light.h"
class DirectionalLight : public Light
{
public:
	DirectionalLight(glm::vec3 position, glm::vec3 color, Shader& shadowShader, int farPlane, int nearPlane);
	virtual ~DirectionalLight();

	void UpdateUniforms(Shader& shader, int lightId, int textureId) override;

	void PreRender() const override;
	void RenderDebug(Shader& shader) const override;
protected:
	glm::mat4 GetProjection() const override;
	glm::mat4 GetShadowMatrix() const;

	GLfloat m_nearPlane;
};

