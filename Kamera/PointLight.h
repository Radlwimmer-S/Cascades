#pragma once
#include <GL/glew.h>
#include <vector>
#include "Light.h"

class Shader;

class PointLight : public Light
{
public:
	PointLight(glm::vec3 position, glm::vec3 color, Shader& shadowShader, GLfloat farPlane, GLfloat nearPlane = 0.1f);
	virtual ~PointLight();

	void UpdateUniforms(Shader& shader, int lightIndex, int textureIndex) override;

	void PreRender() const override;
	void RenderDebug(Shader& shader) const override;
		
protected:
	int GetType() override;
	glm::mat4 GetProjection() const;
	std::vector<glm::mat4> GetShadowMatrices() const;
};

