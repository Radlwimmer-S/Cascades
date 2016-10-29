#pragma once
#include <GL/glew.h>
#include <glm/detail/type_vec3.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Enums.h"

class Shader;

class Light
{
public:
	Light(glm::vec3 position, glm::quat rotation, glm::vec3 color, LightType type = Directional);
	~Light();

	void Activate(Shader& shader) const;
	void Deactivate() const;
	GLuint GetDepthMap() const
	{
		return depthMap;
	}

	glm::vec3 GetColor() const;
	glm::vec3 GetPosition() const;
	void SetPosition(glm::vec3 position);
	void SetRotation(glm::quat rotation);
	glm::mat4 GetLightSpace() const;
protected:
	glm::mat4 GetProjection() const;
	glm::mat4 GetView() const;

	glm::vec3 m_position;
	glm::quat m_rotation;
	glm::vec3 m_color;
	LightType m_type;
	GLuint depthMap;
	GLuint depthMapFBO;
	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
};

