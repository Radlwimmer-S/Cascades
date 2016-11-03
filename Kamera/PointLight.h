#pragma once
#include <GL/glew.h>
#include <vector>
#include "Light.h"

class Shader;

class PointLight : public Light
{
public:
	PointLight(glm::vec3 position, glm::vec3 color, Shader& shadowShader, GLfloat farPlane);
	virtual ~PointLight();

	void UpdateUniforms(Shader& shader, LightIndexer& indizes) override;

	void PreRender() const override;
	void RenderDebug(Shader& shader) const override;
		
protected:
	glm::mat4 GetProjection() const override;
	std::vector<glm::mat4> GetShadowMatrices() const;
};

