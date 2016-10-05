#pragma once
#include "Path.h"
#include <vector>
#include <glm/gtx/quaternion.hpp>

struct ControlPoint
{
	ControlPoint(glm::vec3 position, glm::quat rotation) : Position(position), Rotation(rotation), TimeStamp(0)
	{}

	glm::vec3 Position;
	glm::quat Rotation;
	GLfloat TimeStamp;
};

class CameraPath
{
public:
	CameraPath(std::vector<ControlPoint>& controlPoints, GLuint duration);
	~CameraPath();
	void Initialize();
	virtual void Update(GLfloat deltaTime);
	virtual void Render(Shader& shader) const;

	virtual glm::vec3 GetPosByTime(GLfloat timestamp) const;

	glm::vec3 GetPosition() const
	{
		return m_position + glm::vec3(0, 2, 0);
	}

	glm::quat GetRotation() const
	{
		return glm::quat(0.0f, 0.0f, -1.0f, 0.0f);
	}

	GLfloat GetDuration()const
	{
		return m_duration;
	}
private:
	void CalculateTimeSteps();
	void CalculateApprox();
	GLuint GetIndex(GLfloat timestamp) const;
	glm::vec3 CatmullRomSpline(const std::vector<ControlPoint>& cp, float t) const;

	std::vector<ControlPoint>& m_controlPoints;
	GLuint m_controlCount;

	glm::vec3* m_pathApprox;
	GLuint m_vao;

	GLuint m_index;
	GLfloat m_timer;

	GLuint m_duration;
	glm::vec3 m_position;
};

