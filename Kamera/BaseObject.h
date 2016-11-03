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

	void SetPosition(glm::vec3 position);
	glm::vec3 GetPosition() const;
	void Move(glm::vec3 direction, GLfloat speed);

	void SetOrientation(glm::quat orientation);
	glm::quat GetOrientation() const;
	void Rotate(glm::quat rotation);
	
	virtual void ProcessInput(GLFWwindow& window);
	virtual bool KeyCallback(int key, int scancode, int action, int mode);
	virtual bool CursorPosCallback(float x, float y);

	glm::mat4 GetMatrix() const;

protected:
	glm::vec3 m_position;
	glm::quat m_orientation;
};

