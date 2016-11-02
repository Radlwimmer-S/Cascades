#include "BaseObject.h"
#include <GLFW/glfw3.h>

BaseObject::BaseObject(glm::vec3 position) : m_position(position), m_orientation(glm::quat())
{
}

BaseObject::BaseObject(glm::vec3 position, glm::quat orientaton) : m_position(position), m_orientation(orientaton)
{
}

BaseObject::~BaseObject()
{
}

void BaseObject::SetPosition(glm::vec3 position)
{
	m_position = position;
}

glm::vec3 BaseObject::GetPosition() const
{
	return m_position;
}

void BaseObject::Move(glm::vec3 direction, GLfloat speed)
{
	direction = direction * speed;
	direction = direction * m_orientation;
	m_position += direction;
}

void BaseObject::SetOrientation(glm::quat orientation)
{
	m_orientation = orientation;
}

glm::quat BaseObject::GetOrientation() const
{
	return m_orientation;
}

void BaseObject::Rotate(glm::quat rotation)
{
	m_orientation = glm::normalize(glm::quat(rotation) * m_orientation);
}

void BaseObject::ProcessInput(GLFWwindow& window)
{
	if (glfwGetKey(&window, GLFW_KEY_W))
		Move(glm::vec3(0, 0, -1), KEY_SENSITIVITY);
	if (glfwGetKey(&window, GLFW_KEY_S))
		Move(glm::vec3(0, 0, 1), KEY_SENSITIVITY);
	if (glfwGetKey(&window, GLFW_KEY_A))
		Move(glm::vec3(-1, 0, 0), KEY_SENSITIVITY);
	if (glfwGetKey(&window, GLFW_KEY_D))
		Move(glm::vec3(1, 0, 0), KEY_SENSITIVITY);
	if (glfwGetKey(&window, GLFW_KEY_LEFT_CONTROL))
		Move(glm::vec3(0, -1, 0), KEY_SENSITIVITY);
	if (glfwGetKey(&window, GLFW_KEY_SPACE))
		Move(glm::vec3(0, 1, 0), KEY_SENSITIVITY);
}

bool BaseObject::KeyCallback(int key, int scancode, int action, int mode)
{
	return false;
}

bool BaseObject::CursorPosCallback(float x, float y)
{
	return false;
}
