#pragma once
#include <GL/glew.h>
#include <glm/detail/type_vec3.hpp>
#include <vector>
#include "Model.h"

class Shader;

class Light : public BaseObject
{
public:
	Light(glm::vec3 position, glm::vec3 color, GLfloat farPlane);
	~Light();

	void PreRender(Shader& shader) const;
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

	glm::vec3 GetColor() const;
	std::vector<glm::mat4> GetShadowMatrices() const;
protected:
	glm::mat4 GetProjection() const;

	glm::vec3 m_color;
	GLuint depthCubemap;
	GLuint depthMapFBO;
	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	BaseObject* m_debugCube;
	GLfloat m_farPlane;
};

