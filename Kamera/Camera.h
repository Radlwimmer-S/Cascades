#pragma once
#include <glm/glm.hpp>
#include "CameraPath.h"
#include "ViewPath.h"

enum CameraMode
{
	Overview,
	Manual,
	Automatic
};

class Camera
{
public:
	Camera(glm::vec3 position, CameraPath& path, ViewPath& view);
	~Camera();

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
	CameraPath& m_path;
	ViewPath& m_view;
};

