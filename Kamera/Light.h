#pragma once
#include "BaseObject.h"
class Light : public BaseObject
{
public:
	Light(glm::vec3 position, glm::quat orientation, glm::vec3 color, Shader& shadowShader, int nearPlane, int farPlane);
	virtual ~Light();

	virtual void UpdateUniforms(Shader& shader, int lightIndex, int textureIndex);

	virtual void PreRender() const = 0;
	virtual void PostRender() const;
	virtual void RenderDebug(Shader& shader) const = 0;
	
	Shader& GetShadowShader() const
	{
		return m_shadowShader;
	}

	virtual int GetType() = 0;

protected:
	glm::vec3 m_color;
	GLuint depthMap;
	GLuint depthMapFBO;
	const GLuint SHADOW_RES = 2;
	const GLuint SHADOW_WIDTH = 1024 * SHADOW_RES, SHADOW_HEIGHT = 1024 * SHADOW_RES;
	Shader& m_shadowShader;
	GLfloat m_farPlane;
	GLfloat m_nearPlane;

	BaseObject* m_debugCube;
};

