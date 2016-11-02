#pragma once
#include <GL/glew.h>
#include <glm/detail/type_vec3.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Enums.h"
#include <vector>

class Shader;

class Light
{
public:
	Light(glm::vec3 position, glm::quat rotation, glm::vec3 color, LightType type = Directional);
	~Light();

	void PreRender(Shader& shader) const;
	void ConfigureShader(Shader& shader) const;
	void PostRender() const;
	GLuint GetDepthMap() const
	{
		return depthCubemap;
	}

	static GLfloat GetFarPlane()
	{
		return 25.0f;
	}

	glm::vec3 GetColor() const;
	glm::vec3 GetPosition() const;
	void SetPosition(glm::vec3 position);
	void SetRotation(glm::quat rotation);
	std::vector<glm::mat4> GetShadowMatrices() const;
protected:
	glm::mat4 GetProjection() const;

	glm::vec3 m_position;
	glm::quat m_rotation;
	glm::vec3 m_color;
	LightType m_type;
	GLuint depthCubemap;
	GLuint depthMapFBO;
	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
};

