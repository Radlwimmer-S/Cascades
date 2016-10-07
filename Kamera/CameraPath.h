#pragma once
#include <vector>
#include <glm/gtx/quaternion.hpp>
#include <GL/glew.h>

class Shader;

struct ControlPoint
{
	ControlPoint() : Position(glm::vec3()), Rotation(glm::quat()), TimeStamp(0)
	{}
	ControlPoint(glm::vec3 position, glm::quat rotation) : Position(position), Rotation(rotation), TimeStamp(0)
	{}
	ControlPoint(glm::vec3 position, glm::quat rotation, GLfloat timeStamp) : Position(position), Rotation(rotation), TimeStamp(timeStamp)
	{}

	glm::vec3 Position;
	glm::quat Rotation;
	GLfloat TimeStamp;
};

class CameraPath
{
public:
	CameraPath(std::vector<ControlPoint>& controlPoints);
	CameraPath(std::vector<ControlPoint>& controlPoints, GLuint duration);
	~CameraPath();
	void Initialize(bool recalculateTimeSteps);
	virtual void Update(GLfloat deltaTime);
	virtual void Render(Shader& shader) const;

	virtual glm::vec3 GetPosByTime(GLfloat timestamp) const;
	virtual glm::quat GetRotByTime(GLfloat timestamp) const;

	glm::vec3 GetPosition() const
	{
		return m_position;
	}

	glm::quat GetRotation() const
	{
		return m_rotation;
	}

	GLfloat GetDuration()const
	{
		return m_duration;
	}
private:
	void DoubleControlPoints();
	void CalculateTimeSteps();
	void CalculateApprox();
	GLfloat NormalizeTimeStamp(GLfloat timestamp) const;
	GLuint GetIndex(GLfloat timestamp) const;
	glm::vec3 CatmullRomSpline(const std::vector<ControlPoint>& cp, float t) const;
	glm::quat Squad(const std::vector<ControlPoint>& cp, float t) const;

	std::vector<ControlPoint>& m_controlPoints;
	GLuint m_controlCount;

	glm::vec3* m_pathApprox;
	GLuint m_vao;

	GLuint m_index;
	GLfloat m_timer;

	GLuint m_duration;
	glm::vec3 m_position;
	glm::quat m_rotation;
};

