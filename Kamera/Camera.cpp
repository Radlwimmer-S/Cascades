#include "Camera.h"
#include "Global.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"
#include <GLFW/glfw3.h>

Camera::Camera() : m_mode(Overview), m_position(glm::vec3(0, 0, 0)), m_path(nullptr), m_renderPath(false)
{
}

Camera::Camera(CameraPath* path) : m_mode(Automatic), m_position(glm::vec3(0, 0, 0)), m_path(path), m_renderPath(false)
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


		break;
	}
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
	glm::mat4 rotation = glm::mat4_cast(glm::normalize(m_rotation));

	return rotation * translation;
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
		}

	return false;
}

bool Camera::CursorPosCallback(double x, double y)
{
	return false;
}
