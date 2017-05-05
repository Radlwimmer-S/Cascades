#pragma once
#include <glm/detail/type_vec3.hpp>
#include <vector>
#include <glm/gtx/quaternion.hpp>
#include <GL/glew.h>
#include "Model.h"

struct ControlPoint
{
	ControlPoint() : Position(glm::vec3()), Orientation(glm::vec3()), TimeStamp(0)
	{}
	ControlPoint(glm::vec3 position, glm::vec3 rotation) : Position(position), Orientation(rotation), TimeStamp(0)
	{}
	ControlPoint(glm::vec3 position, glm::vec3 rotation, GLfloat timeStamp) : Position(position), Orientation(rotation), TimeStamp(timeStamp)
	{}

	glm::vec3 Position;
	glm::vec3 Orientation;
	GLfloat TimeStamp;
};
class BasePath
{
public:
	BasePath(std::vector<ControlPoint>& controlPoints, GLuint duration);;
	virtual ~BasePath();
	virtual void Update(GLfloat deltaTime);
	virtual void Render(Shader& shader) const;

	glm::vec3 GetPosition() const
	{
		return m_position;
	}

	glm::quat GetRotation() const
	{
		return m_rotation;
	}

	GLfloat GetDuration() const
	{
		return m_duration;
	}

protected:
	glm::vec3 GetPosByTime(GLfloat timestamp) const;
	glm::quat GetRotByTime(GLfloat timestamp) const;
	void CalculateApprox();
	GLfloat NormalizeTimeStamp(GLfloat timestamp) const;
	GLuint GetIndex(GLfloat timestamp) const;
	virtual glm::vec3 CatmullRomSpline(const std::vector<ControlPoint>& cp, float t) const = 0;
	virtual glm::quat Squad(const std::vector<ControlPoint>& cp, float t) const = 0;

	static glm::vec3 CatmullRomSpline(ControlPoint cp0, ControlPoint cp1, ControlPoint cp2, ControlPoint cp3, float t);
	static glm::quat Squad(ControlPoint cp0, ControlPoint cp1, ControlPoint cp2, ControlPoint cp3, float t);

	GLuint m_duration;
	glm::vec3 m_position;
	glm::quat m_rotation;

	std::vector<ControlPoint>& m_controlPoints;
	GLuint m_controlCount;

	GLuint m_vao;

	GLfloat m_timer;

	Model* m_debugCube;
};

