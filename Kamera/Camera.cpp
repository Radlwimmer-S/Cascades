#include "Camera.h"
#include "Global.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"

Camera::Camera() : m_mode(Overview), m_position(glm::vec3(0, 0, 0)), m_path(nullptr)
{
}

Camera::Camera(CameraPath* path) : m_mode(Overview), m_position(glm::vec3(0, 0, 0)), m_path(path)
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
		m_position = glm::vec3(0.0f, 2.0f, -10.0f);//glm::vec3(0.0f, 10.0f, 0.0f);
		m_rotation = glm::quat(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	case Automatic:
		m_position = m_path->GetPosition();
		m_rotation = m_path->GetRotation();
		break;
	case Manual:


		break;
	}
}

void Camera::Render(Shader& shader)
{
	m_path->Render(shader);
	switch (m_mode)
	{
	case Overview:
		break;
	case Automatic:
		break;
	case Manual:
		break;
	}
}

void Camera::ProcessInput(GLfloat deltaTime)
{
}

glm::mat4 Camera::GetViewMatrix() const
{
	glm::mat4 translation = glm::mat4(1.0f);
	translation = glm::translate(translation, m_position);
	glm::mat4 rotation = glm::mat4_cast(glm::normalize(m_rotation));

	return rotation * translation;
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return glm::perspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
}

