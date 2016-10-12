#pragma once
#include <glm/glm.hpp>
#include "CameraPath.h"
#include <glm/gtc/quaternion.hpp>

class Shader;

enum CameraMode
{
	Overview,
	Manual,
	Automatic
};

enum CameraType
{
	FPS,
	Free
};

class Camera
{
public:
	Camera();
	Camera(CameraPath* path);
	~Camera();

	virtual void Update(GLfloat deltaTime);
	virtual void Render(Shader& shader);

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

	glm::quat GetRotation() const
	{
		return m_rotation;
	}

	void SetPosition(glm::vec3 pos)
	{
		m_position = pos;
	}

	bool KeyCallback(int key, int scancode, int action, int mode);
	bool CursorPosCallback(float x, float y);
protected:

	void ProcessInput();
	void Move(glm::vec3 direction, GLfloat speed);
	void Rotate(glm::quat rot);

	CameraMode m_mode;
	CameraType m_type;
	glm::vec3 m_position; 
	glm::quat m_rotation;
	CameraPath* m_path;
	bool m_renderPath;
	glm::vec2 m_mousePos;
	glm::vec3 m_keyRotation;
	const float m_mouseSensitivity;
};

