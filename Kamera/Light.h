#pragma once
#include "BaseObject.h"
class Light : public BaseObject
{
public:
	Light(glm::vec3 position, glm::quat orientation, glm::vec3 color, Shader& shadowShader, int farPlane);
	virtual ~Light();

	virtual void UpdateUniforms(Shader& shader, LightIndexer& indizes) = 0;

	virtual void PreRender() const = 0;
	virtual void PostRender() const;
	virtual void RenderDebug(Shader& shader) const = 0;
	
	Shader& GetShadowShader() const
	{
		return m_shadowShader;
	}

protected:
	virtual glm::mat4 GetProjection() const = 0;

	glm::vec3 m_color;
	GLuint depthMap;
	GLuint depthMapFBO;
	const GLuint SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
	Shader& m_shadowShader;
	GLfloat m_farPlane;

	BaseObject* m_debugCube;
};

