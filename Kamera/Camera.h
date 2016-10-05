#pragma once
#include <glm/glm.hpp>
#include "CameraPath.h"
#include "ViewPath.h"
#include <glm/gtc/quaternion.hpp>

class Shader;

enum CameraMode
{
	Overview,
	Manual,
	Automatic
};

class Camera
{
public:
	Camera();
	Camera(CameraPath* path);
	~Camera();

	virtual void Update(GLfloat deltaTime);
	virtual void Render(Shader& shader);
	virtual void ProcessInput(GLfloat deltaTime);

	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;

	CameraMode GetMode() const
	{
		return m_mode;
	}

	void SetMode(CameraMode mode)
	{
		m_mode = mode;
	}

	glm::vec3 GetPosition() const
	{
		return m_position;
	}

	void SetPosition(glm::vec3 pos)
	{
		m_position = pos;
	}

private:

	CameraMode m_mode;
	glm::vec3 m_position; 
	glm::quat m_rotation;
	CameraPath* m_path;
};

