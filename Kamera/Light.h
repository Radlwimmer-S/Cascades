#pragma once
#include <GL/glew.h>
#include <glm/detail/type_vec3.hpp>
#include <vector>
#include "Model.h"

class Shader;

class Light : public BaseObject
{
public:
	Light(glm::vec3 position, glm::vec3 color, Shader& shadowShader, GLfloat farPlane);
	~Light();

	void UpdateUniforms(Shader& shader, int lightId, int textureId);

	void PreRender() const;
	void PostRender() const;
	void RenderCube(Shader& shader) const;


	GLuint GetDepthMap() const
	{
		return depthCubemap;
	}

	GLfloat GetFarPlane() const
	{
		return m_farPlane;
	}

	Shader& GetShadowShader() const
	{
		return m_shadowShader;
	}

	glm::vec3 GetColor() const;
	std::vector<glm::mat4> GetShadowMatrices() const;
protected:
	glm::mat4 GetProjection() const;

	glm::vec3 m_color;
	GLuint depthCubemap;
	GLuint depthMapFBO;
	const GLuint SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
	BaseObject* m_debugCube;
	GLfloat m_farPlane;
	Shader& m_shadowShader;
};

