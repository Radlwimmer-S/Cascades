#pragma once
#include <glm/detail/type_vec3.hpp>
#include <GL/glew.h>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Global.h"
#include <GLFW/glfw3.h>

class Shader;

class BaseObject
{
public:
	BaseObject(glm::vec3 position);
	BaseObject(glm::vec3 position, glm::quat orientaton);
	virtual ~BaseObject();
	virtual void Update(GLfloat deltaTime) {}
	virtual void Render(Shader& shader) const {}

	virtual void SetPosition(glm::vec3 position);
	virtual glm::vec3 GetPosition() const;
	virtual void Move(glm::vec3 direction, GLfloat speed);

	virtual void SetOrientation(glm::quat orientation);
	virtual glm::quat GetOrientation() const;
	virtual void Rotate(glm::quat rotation);

	virtual void IsEnabled(bool isEnabled);
	virtual bool IsEnabled() const;
	
	virtual void ProcessInput(GLFWwindow& window);
	virtual bool KeyCallback(int key, int scancode, int action, int mode);
	virtual bool CursorPosCallback(float x, float y);

	virtual glm::mat4 GetMatrix() const;

protected:
	glm::vec3 m_position;
	glm::quat m_orientation;
	bool m_isEnabled;
};
