#include "Camera.h"
#include "Global.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"
#include <GLFW/glfw3.h>

Camera::Camera() : m_mode(Overview), m_type(FPS), m_position(glm::vec3(0, 0, 0)), m_path(nullptr), m_renderPath(false), m_mouseSensitivity(0.1f)
{
}

Camera::Camera(CameraPath* path) : m_mode(Automatic), m_type(Free), m_position(glm::vec3(0, 0, 0)), m_path(path), m_renderPath(false), m_mouseSensitivity(0.1f)
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
		m_rotation = MakeQuad(90, 0, 90);
		break;
	case Automatic:
		m_position = m_path->GetPosition() + glm::vec3(0, 0.25f, 0);
		m_rotation = m_path->GetRotation();
		break;
	case Manual:
		ProcessInput();
		break;
	}
	m_keyRotation = glm::vec3();
}

void Camera::Render(Shader& shader)
{
	if (m_renderPath)
		m_path->Render(shader);
}

glm::mat4 Camera::GetViewMatrix() const
{
	glm::mat4 translation = glm::mat4(1.0f);
	translation = glm::translate(translation, -m_position);
	glm::mat4 rotation = glm::toMat4(glm::normalize(m_rotation));

	return rotation * translation;

	glm::vec3 center = glm::normalize(glm::vec3(0, 0, -1) * m_rotation);
	glm::vec3 up = glm::normalize(glm::vec3(0, 1, 0) * m_rotation);
	return glm::lookAt(m_position, m_position + center, up);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return glm::perspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
}

bool Camera::KeyCallback(int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
		switch (key)
		{
		case GLFW_KEY_1:
			if (m_mode != Overview)
				m_mode = Overview;
			else
				m_mode = Automatic;
			return true;
		case GLFW_KEY_2:
			m_renderPath = !m_renderPath;
			return true;
		case GLFW_KEY_3:
			if (m_type != Free)
				m_type = Free;
			else
				m_type = FPS;
			return true;
		}
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
		switch (key)
		{
		case GLFW_KEY_W:
			m_mode = Manual;
			Move(glm::vec3(0, 0, -1), 1);
			return true;
		case GLFW_KEY_A:
			m_mode = Manual;
			Move(glm::vec3(-1, 0, 0), 1);
			return true;
		case GLFW_KEY_S:
			m_mode = Manual;
			Move(glm::vec3(0, 0, 1), 1);
			return true;
		case GLFW_KEY_D:
			m_mode = Manual;
			Move(glm::vec3(1, 0, 0), 1);
			return true;
		case GLFW_KEY_Q:
			m_mode = Manual;
			m_keyRotation.z += glm::radians(-10.0f);
			return true;
		case GLFW_KEY_E:
			m_mode = Manual;
			m_keyRotation.z += glm::radians(10.0f);
			return true;
		}

	return false;
}

bool Camera::CursorPosCallback(float x, float y)
{
	if (m_mousePos == glm::vec2())
		m_mousePos = glm::vec2(x, y);

	glm::vec2 newMousePos = glm::vec2(x, y);
	glm::vec2 delta = newMousePos - m_mousePos;

	if (m_mode == Manual)
	{
		m_keyRotation.y += glm::radians(delta.x * m_mouseSensitivity);
		m_keyRotation.x += glm::radians(delta.y * m_mouseSensitivity);
	}

	m_mousePos = newMousePos;

	return true;
}

void Camera::ProcessInput()
{
	switch (m_type)
	{
	case FPS:
		m_rotation = glm::normalize(glm::quat(glm::vec3(m_keyRotation.x, 0, 0)) * glm::quat(glm::vec3(0, 0, m_keyRotation.z)) * m_rotation * glm::quat(glm::vec3(0, m_keyRotation.y, 0)));
		break;
	case Free:
		m_rotation = glm::normalize(glm::quat(m_keyRotation) * m_rotation);
		break;
	}
}

void Camera::Move(glm::vec3 direction, GLfloat speed)
{
	direction = direction * speed;
	direction = direction * m_rotation;
	m_position += direction;
}
