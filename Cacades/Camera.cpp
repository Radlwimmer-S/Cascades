#include "Camera.h"
#include "Global.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"
#include <GLFW/glfw3.h>

Camera::Camera() : BaseObject(glm::vec3(0, 0, 0), MakeQuad(0, 0, 0)), m_mode(Manual), m_type(FPS), m_path(nullptr), m_renderPath(false)
{
}

Camera::Camera(CameraPath* path) : BaseObject(glm::vec3(0, 10, 10), MakeQuad(45, 0, 0)), m_mode(Manual), m_type(FPS), m_path(path), m_renderPath(false)
{
}


Camera::~Camera()
{
}


void Camera::Update(GLfloat deltaTime)
{
	if (m_path != nullptr)
	{
		m_path->Update(deltaTime);
	}
	switch (m_mode)
	{
	case Overview:
		m_position = glm::vec3(0.0f, 20.0f, 0.0f);
		m_orientation = MakeQuad(90, 0, 0);
		break;
	case Automatic:
		if (m_path == nullptr)
		{
			m_mode = Manual;
			break;
		}

		m_position = m_path->GetPosition() + glm::vec3(0, 0.25f, 0);
		m_orientation = m_path->GetRotation();
		break;
	case Manual:
		break;
	}
}

void Camera::Render(Shader& shader)const
{
	if (m_renderPath)
		m_path->Render(shader);
}

void Camera::Move(glm::vec3 direction, GLfloat speed)
{
	m_mode = Manual;

	BaseObject::Move(direction, speed);
}

glm::mat4 Camera::GetViewMatrix() const
{
	/*glm::mat4 translation = glm::mat4(1.0f);
	translation = glm::translate(translation, -m_position);
	glm::mat4 rotation = glm::toMat4(glm::normalize(m_orientation));

	return rotation * translation;*/

	glm::vec3 center = glm::normalize(glm::vec3(0, 0, -1) * m_orientation);
	glm::vec3 up = glm::normalize(glm::vec3(0, 1, 0) * m_orientation);
	return glm::lookAt(m_position, m_position + center, up);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return glm::perspective(45.0f, (float)WIDTH / (float)HEIGHT, -10.0f, 100.0f);
}

void Camera::SetRenderPath(bool value)
{
	m_renderPath = value;
}

bool Camera::GetRenderPath() const
{
	return m_renderPath;
}

bool Camera::CursorPosCallback(float x, float y)
{
	if (m_mousePos == glm::vec2())
		m_mousePos = glm::vec2(x, y);

	glm::vec2 newMousePos = glm::vec2(x, y);
	glm::vec2 delta = newMousePos - m_mousePos;
	glm::vec3 m_keyRotation = glm::vec3(delta.y, delta.x, 0) * MOUSE_SENSITIVITY;

	if (m_mode == Manual)
		switch (m_type)
		{
		case FPS:
			m_orientation = glm::normalize(glm::quat(glm::vec3(m_keyRotation.x, 0, 0)) * glm::quat(glm::vec3(0, 0, m_keyRotation.z)) * m_orientation * glm::quat(glm::vec3(0, m_keyRotation.y, 0)));
			break;
		case Free:
			m_orientation = glm::normalize(glm::quat(m_keyRotation) * m_orientation);
			break;
		}

	m_mousePos = newMousePos;

	return true;
}
