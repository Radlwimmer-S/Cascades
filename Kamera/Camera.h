#pragma once
#include <glm/glm.hpp>
#include "CameraPath.h"
#include <glm/gtc/quaternion.hpp>
#include "BaseObject.h"

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

class Camera : public BaseObject
{
public:
	Camera();
	Camera(CameraPath* path);
	~Camera();

	void Update(GLfloat deltaTime) override;
	void Render(Shader& shader)const override;

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

	void ProcessInput(GLFWwindow& window) override;
	bool CursorPosCallback(float x, float y) override;
protected:
	CameraMode m_mode;
	CameraType m_type;
	CameraPath* m_path;
	bool m_renderPath;
	glm::vec2 m_mousePos;
};

